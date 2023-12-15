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
	
	const std::string& GetString(int ID) {
		MyMap::const_iterator it = myMap.find(ID);
		if (it == myMap.end()) {
			throw FindException("GetString failed to find ID");
		}
		return it->second;
	}
	
	int main(int argc, char *argv[]) {
		try {
			const std::string& s1 = GetString(1);
			const std::string& s2 = GetString(3);
			std::cout << s1 << std::endl;
			std::cout << s2 << std::endl;
		}
		catch (FindException& e) {
			std::cout << e.what() << std::endl;
			return 1;
		}
		return 0;
	}
