//g++ -o regex_testcase_2 regex_testcase_2.cpp

#include <fstream>
#include <iostream>
#include <boost/regex.hpp>

int main(int argc, char *argv[]) {
	std::ifstream conf("/sdf1/compatible");
/*
	std::string line = "ambarella,cv25/ ";
*/
	std::string line;
        std::string chip_ambarella = "ambarella";
    
	getline(conf, line);

        if (line.substr(0, chip_ambarella.length()) == chip_ambarella) {
		std::cout << "ambarella found!" << std::endl;
	}
}
