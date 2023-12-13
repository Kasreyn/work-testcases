#include <unordered_map>
#include <iostream>

#define KV_PAIR(key, value) {key, value}

std::unordered_map<int, std::string> myMap{
	KV_PAIR(1, "One Thousand"),
	KV_PAIR(2, "One Thousand")
};

std::string& GetString(int ID) {
	if (!myMap.contains(ID)) {
		return *(std::string*)nullptr;
	}

	return myMap.at(ID);
}

int main(int argc, char *argv[]) {
	std::string& s1 = GetString(1);
	std::cout << s1 << std::endl;

	std::string& s2 = GetString(3);

	return 0;
}
