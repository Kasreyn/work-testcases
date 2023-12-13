	#include <unordered_map>
	#include <iostream>
	
	std::unordered_map<int, std::string> myMap{
	    {1, "One Thousand"},
	    {2, "One Thousand"}
	};
	
	std::string* GetString(int ID) {
	    if (!myMap.contains(ID)) {
	        return nullptr;
	    }
	    return &myMap.at(ID);
	}
	
	int main(int argc, char *argv[]) {
	    std::string* p1 = GetString(1);
	    std::string* p2 = GetString(2);
	    if (!p1 || !p2) {
	        return 1;
	    }
	    std::string& s1 = *p1;
	    std::string& s2 = *p2;
	    std::cout << s1 << std::endl;
	    std::cout << s2 << std::endl;
	    return 0;
	}
