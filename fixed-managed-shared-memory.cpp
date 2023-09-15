// g++ -std=c++20 -pthread fixed-managed-shared-memory.cpp -o fixed-managed-shared-memory -lrt

#include <boost/interprocess/managed_shared_memory.hpp>

template <typename T>
using Ptr = std::shared_ptr<T>;

template <typename T>
struct Test {
	T value;
};

typedef Test<int> TestInt1;
using TestInt2 = Test<int>;

template<typename M, typename K, typename T>  
const T GetValue(const M& map, const K& key, const T& def) {
	using itype = M::const_iterator; 
//    typename M::const_iterator it = map.find(key);
	itype it = map.find(key);
    return (it == map.end() ? def : it->second);
} 

int main() {
    boost::interprocess::shared_memory_object::remove("MySharedMemory"); // Make sure to clean up any existing shared memory first
    boost::interprocess::fixed_managed_shared_memory segment(boost::interprocess::open_or_create, "MySharedMemory", 65536, (void*)0x30000000);

//    BidirectionalMapContainer *bidiMapContainer = segment.construct<BidirectionalMapContainer>("BidirectionalMap")(segment.get_segment_manager());
}
