#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/impl/read.hpp>
#include <boost/asio/impl/write.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/io/ios_state.hpp>
#include <chrono>
#include <cstring>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "Fine-Grained-Locking-Common.hpp"

namespace bip = boost::interprocess;

void readShmData(unsigned char* data) {
	std::cout << "Client: New data read: ";
	boost::io::ios_flags_saver ifs(std::cout);
	for (std::size_t i = 0; i < 10; ++i) {
		std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << +data[i] << " ";
	}
	std::cout << std::endl;
}

int main() {
	try {
		boost::asio::io_context io;
		boost::asio::ip::tcp::socket socket(io);
		boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
		socket.connect(boost::asio::ip::tcp::endpoint(address, 12345));

		std::stringstream ss;
		ss << "client_sync_" << std::this_thread::get_id();
		std::string shm_name = ss.str();
		bip::shared_memory_object::remove(shm_name.c_str());
		bip::managed_shared_memory shm(bip::create_only, shm_name.c_str(), 4096);
		ClientSync* cs = shm.construct<ClientSync>("sync")();
		std::strncpy(cs->shmName, shm_name.c_str(), shm_name.size() + 1);

		unsigned int size = shm_name.size();
		boost::asio::write(socket, boost::asio::buffer(&size, 4));
		boost::asio::write(socket, boost::asio::buffer(shm_name.data(), shm_name.size()));

		boost::asio::read(socket, boost::asio::buffer(&size, sizeof(size)));
		std::vector<char> buf(size);
		boost::asio::read(socket, boost::asio::buffer(buf.data(), buf.size()));
		std::string srvShm = std::string(buf.begin(), buf.end());
		std::cout << "Client: The server has sent us its shared memory name: " << srvShm << std::endl;

		bip::managed_shared_memory region(bip::open_only, srvShm.c_str());
		std::pair<unsigned char*, std::size_t> p = region.find<unsigned char>("server buffer");
		unsigned char* data						 = p.first;

		while (true) {
			{
				bip::sharable_lock<bip::interprocess_upgradable_mutex> lock(cs->mutex);
				std::cout << "Client " << std::this_thread::get_id() << " reading shared data..."
						  << std::endl;
				readShmData(data);
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			std::cout << " done" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
	catch (std::exception& e) {
		std::cerr << "Client error: " << e.what() << "\n";
	}
}
