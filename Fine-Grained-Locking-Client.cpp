#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/impl/read.hpp>
#include <boost/asio/impl/write.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/xsi_key.hpp>
#include <boost/interprocess/xsi_shared_memory.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/system/detail/error_code.hpp>
#include <chrono>
#include <csignal>
#include <cstring>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <utility>

#include "Fine-Grained-Locking-Common.hpp"

namespace bip = boost::interprocess;

void DoWorkTimer(boost::asio::steady_timer& timer, std::function<void()> f) {
	timer.expires_after(std::chrono::milliseconds(50));
	timer.async_wait([&timer, f](const boost::system::error_code& ec) {
		if (ec) {
			return;
		}
		f();
		DoWorkTimer(timer, f);
	});
}

int main(int argc, char* argv[]) {
	boost::asio::io_context io;
	boost::asio::ip::tcp::socket socket(io);
	boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
	boost::asio::steady_timer timer(io);
	unsigned int shmid_buf;
	unsigned int uid = getuid();
	unsigned int shmid_cli;
	unsigned char* data;
	ClientSync* clientSync;

	int opt;
	unsigned int sleepLen = 1;
	while ((opt = getopt(argc, argv, "l:")) != -1) {
		switch (opt) {
			case 'l':
				std::stringstream ss;
				ss << optarg;
				ss >> sleepLen;
				break;
		}
	}

	try {
		socket.connect(boost::asio::ip::tcp::endpoint(address, 12345));

		boost::asio::read(socket, boost::asio::buffer(&shmid_buf, sizeof(shmid_buf)));
		std::cout << "Client: The server has sent us a shared memory buffer ID: " << shmid_buf << std::endl;
		boost::asio::write(socket, boost::asio::buffer(&uid, sizeof(uid)));
		std::cout << "Client: Sending UID: " << uid << std::endl;
		boost::asio::read(socket, boost::asio::buffer(&shmid_cli, sizeof(shmid_cli)));
		std::cout << "Client: The server has sent us a shared memory client ID: " << shmid_cli << std::endl;
	}
	catch (std::exception& e) {
		std::cerr << "Client error: " << e.what() << "\n";
		return 1;
	}

	bip::xsi_shared_memory xsm_buf(bip::open_only, shmid_buf);
	bip::mapped_region mr_buf(xsm_buf, bip::read_only);
	bip::managed_external_buffer meb_buf(bip::open_only, mr_buf.get_address(), mr_buf.get_size());
	std::pair<unsigned char*, std::size_t> p_buf = meb_buf.find<unsigned char>("server buffer");
	data										 = p_buf.first;

	bip::xsi_shared_memory xsm_cli(bip::open_only, shmid_cli);
	bip::mapped_region mr_cli(xsm_cli, bip::read_write);
	bip::managed_external_buffer meb_cli(bip::open_only, mr_cli.get_address(), mr_cli.get_size());
	std::pair<ClientSync*, std::size_t> p_cli = meb_cli.find<ClientSync>("sync");
	clientSync								  = p_cli.first;

	auto readShmData = [data, clientSync, sleepLen]() {
		{
			bip::sharable_lock<bip::interprocess_upgradable_mutex> lock(clientSync->mutex);

			std::cout << "Client: Reading shared data: "; // Read lock held
			boost::io::ios_flags_saver ifs(std::cout);
			for (std::size_t i = 0; i < 10; ++i) {
				std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << +data[i] << " ";
			}
			std::cout << std::flush;

			std::this_thread::sleep_for(std::chrono::seconds(sleepLen));
		}
		std::cout << "(done)" << std::endl; // Read lock released

		std::this_thread::sleep_for(std::chrono::seconds(1));
	};

	boost::asio::signal_set stopSignals(io, SIGINT, SIGTERM, SIGHUP);
	stopSignals.async_wait([&](const boost::system::error_code& error, int signalNumber) {
		std::cout << "Got signal " << signalNumber << std::endl;
		io.stop();
		if (error.failed()) {
			std::cout << "Error " << error.value() << std::endl;
		}
	});

	DoWorkTimer(timer, readShmData);
	io.run();
}
