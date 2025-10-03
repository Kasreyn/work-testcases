#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <vector>

struct Data {
    std::vector<int> values;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & values;
    }
};
