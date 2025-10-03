#include <iostream>
#include <cstdint>
#include <fstream>
#include <unistd.h>
#include <array>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <malloc.h> 

class ServiceIMPL {
    std::array<uint8_t, 230000> m_StaticData;

    public:
    int Func() {
        for(int i = 0; i < m_StaticData.size() - 1; ++i) {
            m_StaticData[i] = m_StaticData[i + 1];
        }
        return m_StaticData[0];
    }
};

class ServiceCTL1 {
    ServiceIMPL m_ServiceIMPL;

    public:
    int Func() {
        return m_ServiceIMPL.Func();
    }
};

class ServiceCTL2 {
    std::unique_ptr<ServiceIMPL> m_ServiceIMPL;

    public:
    ServiceCTL2() : m_ServiceIMPL(std::make_unique<ServiceIMPL>()) {}

    int Func() {
        return m_ServiceIMPL->Func();
    }
};

template<typename T>
void printMemoryUsage(T& t) {
    long rss = 0L;
    long page_size_kb = sysconf(_SC_PAGESIZE) / 1024; // in KB
    std::ifstream statm("/proc/self/statm");
    long size, resident;
    statm >> size >> resident;
    rss = resident * page_size_kb;
    std::cout << "Current memory (RSS): " << rss << " KB\n";
    std::cout << "Dummy... " << t.Func();
}

int main() {
    ServiceCTL1 instance;
    //ServiceCTL2 instance;

	malloc_trim(0);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
    printMemoryUsage(instance);

    return instance.Func();
}


