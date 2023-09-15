//g++ -o regex_testcase_3 regex_testcase_3.cpp

#include <fstream>
#include <iostream>
#include <boost/regex.hpp>

int main(int argc, char *argv[]) {
	std::ifstream conf("/sdf1/compatible");
/*
	std::string line = "ambarella,cv25/ ";
*/
	std::string line;
    
	getline(conf, line);

        if (line.find("ambarella", 0) != line.npos) {
		std::cout << "ambarella found!" << std::endl;
	}
}
