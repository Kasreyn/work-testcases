//g++ -o regex_testcase regex_testcase.cpp -lboost_system -lboost_regex

#include <fstream>
#include <iostream>
#include <boost/regex.hpp>

int main(int argc, char *argv[]) {
	std::ifstream conf("/sdf1/compatibl");
	boost::smatch m;
/*
	std::string line = "ambarella,cv25/ ";
*/
	std::string line;
	boost::regex e("^([^,]+)");

	getline(conf, line);
	regex_search (line, m, e);
	std::cout << m[0].str() << std::endl;

	if (m[0].str() == "ambarella") {
		std::cout << "ambarella found!" << std::endl;
	}
}
