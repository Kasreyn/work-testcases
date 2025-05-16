#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/system/detail/error_code.hpp>
#include <csignal>
#include <signal.h>

struct MyStruct {
    char buffer[1024 * 1024 * 1000]; // 1GB
//	MyStruct() {}
};

int main(int argc, char* argv[]) {
	boost::asio::io_service io_service;

	MyStruct* s = new MyStruct;   // Allocates 1GB of address space
	s->buffer[0] = 'A';           // Causes page fault; first 4KB allocated
	s->buffer[4096] = 'B';        // Next 4KB allocated

	for (int i = 0; i < 1024 * 1024 * 50; i++) {
		s->buffer[i] = 'X';
	}

	boost::asio::signal_set stop_signals(io_service, SIGINT, SIGTERM, SIGHUP);
	stop_signals.async_wait([&io_service](const boost::system::error_code& ec, int sig) {
		io_service.stop();
	});

	io_service.run();

	return 0;
}
