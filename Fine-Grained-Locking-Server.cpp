#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/impl/read.hpp>
#include <boost/asio/impl/write.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/sync/lock_options.hpp>
#include <boost/interprocess/xsi_key.hpp>
#include <boost/interprocess/xsi_shared_memory.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/system/detail/error_code.hpp>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <thread>
#include <utility>
#include <vector>

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

int main() {
	boost::asio::io_context io;
	boost::asio::steady_timer timer(io);
	std::list<std::unique_ptr<ClientXSI>> activeClientXSIs;
	std::list<std::unique_ptr<ClientXSI>> timedOutClientXSIs;

	bip::xsi_shared_memory xsm(bip::open_or_create, bip::xsi_key{key_t(0)}, 4096, 0644);
	bip::mapped_region mr(xsm, bip::read_write);
	bip::managed_external_buffer meb(bip::create_only, mr.get_address(), mr.get_size());
	unsigned char* data = meb.construct<unsigned char>("server buffer")[10](0x55);

	auto updateShmData = [data]() {
		for (std::size_t i = 0; i < 10; ++i) {
			data[i] = data[i] + 1;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		std::cout << "Server: New data written: ";
		boost::io::ios_flags_saver ifs(std::cout);
		for (std::size_t i = 0; i < 10; ++i) {
			std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << +data[i] << " ";
		}
		std::cout << std::endl;
	};

	auto DoWork = [&activeClientXSIs, &timedOutClientXSIs, &updateShmData]() {
		std::cout << "Server: Taking exclusive locks before modifying the shared buffer: ";
		std::vector<bip::scoped_lock<bip::interprocess_upgradable_mutex>> locks;

		for (std::list<std::unique_ptr<ClientXSI>>::iterator it = activeClientXSIs.begin();
			 it != activeClientXSIs.end();) {
			ClientXSI* cXSI = it->get();
			auto deadline =
				boost::posix_time::microsec_clock::universal_time() + boost::posix_time::milliseconds(500);
			bip::interprocess_upgradable_mutex& mutex = cXSI->GetMutex();
			bip::scoped_lock<bip::interprocess_upgradable_mutex> lock(mutex, bip::defer_lock);

			if (!lock.timed_lock(deadline)) {
				std::cout << cXSI->GetKey() << " (Timeout detected / Deleted) ";
				timedOutClientXSIs.splice(timedOutClientXSIs.end(), activeClientXSIs, it++);
				continue;
			}

			locks.emplace_back(std::move(lock));
			std::cout << cXSI->GetKey() << " ";
			++it;
		}
		std::cout << std::endl;

		std::cout << "Server: Doing work" << std::endl;
		updateShmData();

		std::cout << "Server: Releasing all client locks" << std::endl;
		locks.clear();

		std::cout << "Server: Idle" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));

		for (std::list<std::unique_ptr<ClientXSI>>::iterator it = timedOutClientXSIs.begin();
			 it != timedOutClientXSIs.end();) {
			ClientXSI* cXSI = it->get();
			if (cXSI->TimeoutExpired()) {
				std::cout << "Server: Making " << cXSI->GetKey() << " active again" << std::endl;
				cXSI->Reset();
				activeClientXSIs.splice(activeClientXSIs.end(), timedOutClientXSIs, it++);
				continue;
			}
			++it;
		}
	};

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

			std::unique_ptr<ClientXSI>& cXSI = activeClientXSIs.emplace_back(std::make_unique<ClientXSI>());
			unsigned int shmid_buf			 = xsm.get_shmid();
			unsigned int uid;
			unsigned int shmid_cli = cXSI->GetKey();

			boost::asio::write(socket, boost::asio::buffer(&shmid_buf, sizeof(shmid_buf)));
			std::cout << "Server: Sending shared memory buffer ID: " << shmid_buf << std::endl;
			boost::asio::read(socket, boost::asio::buffer(&uid, sizeof(uid)));
			std::cout << "Server: The client has sent us a UID: " << uid << std::endl;

			struct shmid_ds buf;
			shmctl(shmid_cli, IPC_STAT, &buf);
			buf.shm_perm.uid  = uid;
			buf.shm_perm.gid  = uid;
			buf.shm_perm.mode = 0600;
			shmctl(shmid_cli, IPC_SET, &buf);

			boost::asio::write(socket, boost::asio::buffer(&shmid_cli, sizeof(shmid_cli)));
			std::cout << "Server: Sending shared memory client ID: " << shmid_cli << std::endl;

			socket.close();
		}
	}
	catch (std::exception& e) {
		std::cerr << "Server error: " << e.what() << std::endl;
		bip::xsi_shared_memory::remove(xsm.get_shmid());
		return 1;
	}

	DoWorkTimer(timer, DoWork);
	io.run();
	bip::xsi_shared_memory::remove(xsm.get_shmid());
}
