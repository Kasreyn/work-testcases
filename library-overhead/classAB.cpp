#include <iostream>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>

class B {
	public:
		std::string m_Name = "B";
		boost::asio::io_context m_ioContext;
		boost::asio::local::stream_protocol::socket m_Socket{m_ioContext};
};

class A {
	public:
		std::string m_Name = "A";
		boost::asio::io_context m_ioContext;
		boost::asio::local::stream_protocol::socket m_Socket{m_ioContext};
};

int main(int argc, char *argv[]) {
	A a;
	B b;
}
