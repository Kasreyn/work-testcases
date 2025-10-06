#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/impl/write.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <cstring>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>

#include "Fine-Grained-Locking-Common.hpp"

namespace bip = boost::interprocess;

int main() {
    try {
        boost::asio::io_context io;
        boost::asio::ip::tcp::socket socket(io);
		boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
        socket.connect(boost::asio::ip::tcp::endpoint(address, 12345));

		std::string shm_name = "client_sync_" + std::to_string(getpid());
		bip::shared_memory_object::remove(shm_name.c_str());
		bip::managed_shared_memory shm(bip::create_only, shm_name.c_str(), 4096);
		ClientSync* cs = shm.construct<ClientSync>("sync")();
		std::strncpy(cs->shmName, shm_name.c_str(), shm_name.size() + 1);
	
		unsigned int size = shm_name.size();
		boost::asio::write(socket, boost::asio::buffer(&size, 4));
        boost::asio::write(socket, boost::asio::buffer(shm_name.data(), shm_name.size()));

		ClientSync *sync = shm.find<ClientSync>("sync").first;
		while (true) {
			{
				bip::sharable_lock<bip::interprocess_upgradable_mutex> lock(sync->mutex);
				std::cout << "Client " << getpid() << " reading shared data...";
				sleep(1);
			}
			std::cout << " done" << std::endl;
			sleep(1);
		}
	} catch (std::exception &e) {
		std::cerr << "Client error: " << e.what() << "\n";
	}
}
