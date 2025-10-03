#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <sstream>
#include <iostream>

#include "SerializeData.hpp"

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io;
        tcp::socket socket(io);
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));

        // Send multiple objects
        for (int i = 0; i < 3; ++i) {
            Data data;
            data.values = {i, i*10, i*100};

            // Serialize
            std::ostringstream oss;
            boost::archive::text_oarchive oa(oss);
            oa << data;
            std::string payload = oss.str();

            // Step 1: Send length prefix
            uint32_t msg_size = payload.size();
            boost::asio::write(socket, boost::asio::buffer(&msg_size, sizeof(msg_size)));

            // Step 2: Send payload
            boost::asio::write(socket, boost::asio::buffer(payload.data(), payload.size()));

            std::cout << "Client sent vector: ";
            for (int v : data.values) std::cout << v << " ";
            std::cout << "\n";
        }

    } catch (std::exception &e) {
        std::cerr << "Client error: " << e.what() << "\n";
    }
}
