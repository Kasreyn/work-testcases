#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>
#include <vector>
#include <iostream>

struct Data {
    std::vector<int> values;

    // Tell Boost how to serialize this struct
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & values;   // Boost knows how to handle std::vector<int>
    }
};

int main() {
    // Create object
    Data d1;
    d1.values = {1, 2, 3, 42};

    // Serialize to a string
    std::ostringstream oss;
    {
        boost::archive::text_oarchive oa(oss);
        oa << d1;
    }

    std::string serialized = oss.str();
    std::cout << "Serialized form:\n" << serialized << "\n";

    // Deserialize back
    Data d2;
    std::istringstream iss(serialized);
    {
        boost::archive::text_iarchive ia(iss);
        ia >> d2;
    }

    std::cout << "Deserialized vector: ";
    for (int v : d2.values) std::cout << v << " ";
    std::cout << "\n";
}
