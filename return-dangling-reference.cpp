#include <unordered_map>
#include <iostream>

class FindException : public std::runtime_error {
	public:
		FindException(const std::string& message) : std::runtime_error(message) {}
};

using MyMap = std::unordered_map<int, std::string>;
MyMap myMap{
	{1, "One Thousand"},
	{2, "One Thousand"}
};

std::string* GetString(int ID) {
	MyMap::const_iterator it = myMap.find(ID);
	if (it == myMap.end()) {
		return nullptr;
	}
	return &myMap.at(ID);
}

int main(int argc, char *argv[]) {
	const std::string* s1 = GetString(1);
	const std::string* s2 = GetString(2);
	if (!s1 || !s2) {
		return 1;
	}
	const std::string& r1 = *s1;
	const std::string& r2 = *s2;
	std::cout << r1 << std::endl;
	std::cout << r2 << std::endl;
	return 0;
}
