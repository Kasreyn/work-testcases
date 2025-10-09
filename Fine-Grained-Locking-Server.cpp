#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/impl/read.hpp>
#include <boost/asio/impl/write.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/detail/os_file_functions.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/xsi_key.hpp>
#include <boost/interprocess/xsi_shared_memory.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/system/detail/error_code.hpp>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <list>

#include "Fine-Grained-Locking-Common.hpp"

namespace bip = boost::interprocess;

void updateShmData(unsigned char* data) {
	for (std::size_t i = 0; i < 10; ++i) {
		data[i] = data[i] + 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	std::cout << "Server: New data written: ";
	boost::io::ios_flags_saver ifs(std::cout);
	for (std::size_t i = 0; i < 10; ++i) {
		std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << +data[i] << " ";
	}
	std::cout << std::endl;
}

void DoWork(boost::asio::steady_timer& timer, std::vector<bip::offset_ptr<ClientSync>>& syncObjects, unsigned char* data) {
    timer.expires_after(std::chrono::seconds(1));
    timer.async_wait([&timer, &syncObjects, data](const boost::system::error_code& ec) {
        if (ec) {
			return;
		}

		std::cout << "Server: Taking exclusive locks before modifying the shared buffer: ";
		std::vector<bip::scoped_lock<bip::interprocess_upgradable_mutex>> locks;
		for (bip::offset_ptr<ClientSync> s : syncObjects) {
			locks.emplace_back(s->mutex);
			std::cout << s->shmName << " ";
		}
		std::cout << std::endl;

		std::cout << "Server: Doing work" << std::endl;
		updateShmData(data);

		std::cout << "Server: Releasing all client locks" << std::endl;
		locks.clear();

		std::cout << "Server: Idle" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));

		DoWork(timer, syncObjects, data);
    });
}

int main() {
	boost::asio::io_context io;
	boost::asio::steady_timer timer(io);
	std::vector<std::string> clientShmNames;
	std::list<ClientXSI> clientXSIs;

	boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_or_create,
											   boost::interprocess::xsi_key{key_t(0)}, 4096, 0640);
	boost::interprocess::mapped_region mr(xsm, boost::interprocess::read_write);
	boost::interprocess::managed_external_buffer meb(boost::interprocess::create_only, mr.get_address(),
													 mr.get_size());

	std::cout << "Shared memory address: " << meb.get_address() << ", size: " << meb.get_size()
			  << std::endl;
	unsigned char* data = meb.construct<unsigned char>("server buffer")[10](0x55);

	boost::asio::signal_set stopSignals(io, SIGINT, SIGTERM, SIGHUP);
	stopSignals.async_wait([&](const boost::system::error_code& error, int signalNumber) {
		std::cout << "Got signal " << signalNumber << std::endl;

		io.stop();

		if (error.failed()) {
			std::cout << "Error " << error.value() << std::endl;
		}
	});

	try {
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 12345);
		boost::asio::ip::tcp::acceptor acceptor(io, endpoint);
		boost::asio::ip::tcp::socket socket(io);

		for (int i = 0; i < 2; i++) {
			acceptor.accept(socket);

			uint32_t msg_size = 0;
			boost::asio::read(socket, boost::asio::buffer(&msg_size, sizeof(msg_size)));

			std::vector<char> buf(msg_size);
			boost::asio::read(socket, boost::asio::buffer(buf.data(), buf.size()));

			std::string& shmName = clientShmNames.emplace_back(buf.begin(), buf.end());
			std::cout << "Server: A client has sent us its shared memory name: " << shmName << std::endl;

			unsigned int shmid = xsm.get_shmid();
			unsigned int uid   = getuid();

			boost::asio::write(socket, boost::asio::buffer(&shmid, 4));
			boost::asio::write(socket, boost::asio::buffer(&uid, 4));

			ClientXSI& cXSI = clientXSIs.emplace_back();
			unsigned int shmid2 = cXSI.GetKey();
			boost::asio::write(socket, boost::asio::buffer(&shmid2, 4));

			socket.close();
		}

		std::vector<bip::managed_shared_memory> regions;
		std::vector<bip::offset_ptr<ClientSync>> syncObjects;
		for (std::string& s : clientShmNames) {
			bip::managed_shared_memory& region = regions.emplace_back(bip::open_only, s.c_str());
			syncObjects.push_back(region.find<ClientSync>("sync").first);
		}

		DoWork(timer, syncObjects, data);

		io.run();
	}
	catch (std::exception& e) {
		std::cerr << "Server error: " << e.what() << std::endl;
	}
}
