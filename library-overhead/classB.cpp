#include <iostream>
#include <boost/asio/posix/stream_descriptor.hpp>

class B {
	public:
		std::string m_Name = "B";
		boost::asio::posix::stream_descriptor* d;
};

int main(int argc, char *argv[]) {
	B b;
}
