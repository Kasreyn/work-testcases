#include <iostream>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/variant.hpp>
#include <sstream>
#include <vector>

int main() {
	boost::variant<std::vector<uint8_t>> nameContainer = std::vector<uint8_t>{ 'A', 'B', 'C'};
	const std::vector<uint8_t>& vec = boost::get<std::vector<uint8_t>>(nameContainer);
//	std::stringstream ss{reinterpret_cast<const char*>(nameRaw.data())};

	std::stringstream ss { std::string(vec.begin(), vec.end()) };
//	ss << &boost::get<std::vector<uint8_t>>(nameContainer)[0];
//	ss << std::string(vec.begin(), vec.end());
	std::cout << ss.str() << std::endl;

//	std::cout << MyNameSpace::myVariable << std::endl;
}

