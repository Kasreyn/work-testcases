//g++ -o regex_testcase regex_testcase.cpp -lboost_system -lboost_regex

#include <fstream>
#include <iostream>
#include <array>

int main(int argc, char *argv[]) {
	std::array<char, 20> arr = { 'h', 'i' };
	std::string s = "trfm_dewarp";
	char s2[20] = "trfm_dewarp";
	arr = std::to_array(s2);
	std::string str(std::begin(arr), std::end(arr));
	std::cout << "Test: " << str << std::endl;
}
