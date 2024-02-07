#include <iostream>
#include <boost/asio/posix/stream_descriptor.hpp>

class B {
	public:
		std::string m_Name = "B";
		boost::asio::posix::stream_descriptor* d;
};

class A {
	public:
		std::string m_Name = "A";
		boost::asio::posix::stream_descriptor* d;
};

int main(int argc, char *argv[]) {
	A a;
	B b;
}
