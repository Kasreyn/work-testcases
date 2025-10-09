#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/impl/read.hpp>
#include <boost/asio/impl/write.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/detail/os_file_functions.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/xsi_key.hpp>
#include <boost/interprocess/xsi_shared_memory.hpp>
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

		unsigned int shmid;
		boost::asio::read(socket, boost::asio::buffer(&shmid, sizeof(shmid)));
		std::cout << "Client: The server has sent us its shared memory ID: " << shmid << std::endl;

		unsigned int uid;
		boost::asio::read(socket, boost::asio::buffer(&uid, 4));
		std::cout << "Client: The server has sent us its UID: " << uid << std::endl;

		unsigned int shmid2;
		boost::asio::read(socket, boost::asio::buffer(&shmid2, sizeof(shmid2)));
		std::cout << "Client: The server has sent us its shared memory ID2: " << shmid2 << std::endl;

		boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_only, shmid);
		boost::interprocess::mapped_region mr(xsm, boost::interprocess::read_only);
		boost::interprocess::managed_external_buffer meb(boost::interprocess::open_only, mr.get_address(),
														 mr.get_size());
		std::pair<unsigned char*, std::size_t> p = meb.find<unsigned char>("server buffer");
		unsigned char* data						 = p.first;

		boost::interprocess::xsi_shared_memory xsm2(boost::interprocess::open_only, shmid2);
		boost::interprocess::mapped_region mr2(xsm2, boost::interprocess::read_only);
		boost::interprocess::managed_external_buffer meb2(boost::interprocess::open_only, mr2.get_address(),
														 mr2.get_size());
//		std::pair<ClientSync*, std::size_t> p2 = meb2.find<ClientSync>("sync");
//		ClientSync* clientSync						 = p2.first;
//		bip::sharable_lock<bip::interprocess_upgradable_mutex> lock(clientSync->mutex);

		while (true) {
			{
//				bip::sharable_lock<bip::interprocess_upgradable_mutex> lock(cs->mutex);
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
