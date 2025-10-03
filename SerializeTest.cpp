#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <sstream>
#include <vector>
#include <iostream>

struct Data {
    std::vector<int> values;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & values;
    }
};

int main() {
    Data d1;
    d1.values = {1, 2, 3, 42};

    std::ostringstream oss;
	boost::archive::text_oarchive oa(oss);
	oa << d1;
    std::string serialized = oss.str();
    std::cout << "Serialized form:\n" << serialized << "\n";
}
