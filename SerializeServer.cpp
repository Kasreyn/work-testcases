#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <sstream>

#include "SerializeData.hpp"

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 12345));
        tcp::socket socket(io);
re_accept:
        acceptor.accept(socket);

        while (true) {
			try {
				// Step 1: Read 4-byte length prefix
				uint32_t msg_size = 0;
				boost::asio::read(socket, boost::asio::buffer(&msg_size, sizeof(msg_size)));

				// Step 2: Read payload of exactly msg_size bytes
				std::vector<char> buf(msg_size);
				boost::asio::read(socket, boost::asio::buffer(buf.data(), buf.size()));

				// Step 3: Deserialize
				std::istringstream iss(std::string(buf.data(), buf.size()));
				boost::archive::text_iarchive ia(iss);

				Data received;
				ia >> received;

				std::cout << "Server received vector: ";
				for (int v : received.values) std::cout << v << " ";
				std::cout << "\n";
			}
			catch (boost::system::system_error &e) {
				if (e.code() == boost::asio::error::eof) {
					std::cout << "Client closed connection\n";
					socket.close();
					goto re_accept;
				} else {
					std::cerr << "Server error: " << e.what() << "\n";
					break;
				}
			}
        }
    } catch (std::exception &e) {
        std::cerr << "Server error: " << e.what() << "\n";
    }
}
