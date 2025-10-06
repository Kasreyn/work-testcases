#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/impl/read.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <chrono>
#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

#include "Fine-Grained-Locking-Common.hpp"

namespace bip = boost::interprocess;

int main() {
	try {
		boost::asio::io_context io;
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 12345);
		boost::asio::ip::tcp::acceptor acceptor(io, endpoint);
		boost::asio::ip::tcp::socket socket(io);
		std::vector<std::string> clientShmNames;

		for (int i = 0; i < 2; i++) {
			acceptor.accept(socket);

			uint32_t msg_size = 0;
			boost::asio::read(socket, boost::asio::buffer(&msg_size, sizeof(msg_size)));
	
			std::vector<char> buf(msg_size);
			boost::asio::read(socket, boost::asio::buffer(buf.data(), buf.size()));

			std::string& shmName = clientShmNames.emplace_back(buf.begin(), buf.end());
			std::cout << "Server: A client has sent us its shared memory name: " << shmName << std::endl;

			socket.close();
		}

		std::vector<bip::managed_shared_memory> regions;
		std::vector<bip::offset_ptr<ClientSync>> syncObjects;
		for (std::string& s : clientShmNames) {
			bip::managed_shared_memory& region = regions.emplace_back(bip::open_only, s.c_str());
			syncObjects.push_back(region.find<ClientSync>("sync").first);
		}

		while (true) {
			std::cout << "Server: Taking exclusive locks before modifying the shared buffer: ";

			std::vector<bip::scoped_lock<bip::interprocess_upgradable_mutex>> locks;
			for (bip::offset_ptr<ClientSync> s : syncObjects) {
				locks.emplace_back(s->mutex);
				std::cout << s->shmName << " ";
			}
			std::cout << std::endl;

			std::cout << "Server: Doing work" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));

			std::cout << "Server: Releasing all client locks" << std::endl;
			locks.clear();

			std::cout << "Server: Idle" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	} catch (std::exception &e) {
		std::cerr << "Server error: " << e.what() << std::endl;
	}
}
