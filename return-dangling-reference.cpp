	#include <unordered_map>
	#include <iostream>
	
	std::unordered_map<int, std::string> myMap{
		{1, "One Thousand"},
		{2, "One Thousand"}
	};
	
	std::pair<std::string&, bool> GetString(int ID) {
		if (!myMap.contains(ID)) {
			return {*(std::string*)nullptr, false};
		}
		return {myMap.at(ID), true};
	}
	
	int main(int argc, char *argv[]) {
		std::pair<std::string&, bool> p1 = GetString(1);
		std::pair<std::string&, bool> p2 = GetString(2);
		if (!p1.second || !p2.second) {
			return 1;
		}
		std::string& s1 = p1.first;
		std::string& s2 = p2.first;
		std::cout << s1 << std::endl;
		std::cout << s2 << std::endl;
		return 0;
	}
