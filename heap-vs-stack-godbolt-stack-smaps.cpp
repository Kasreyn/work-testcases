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
    long size, kernelpagesize, mmupagesize, rss;
    t.Func();
    std::ifstream statm("/proc/self/smaps");
    std::cout << statm.rdbuf();
    statm >> size >> kernelpagesize >> mmupagesize >> rss;
    std::cout << "Current memory (RSS): " << rss << " KB\n";
}

int main() {
    //ServiceCTL1 instance;
    ServiceCTL2 instance;
    printMemoryUsage(instance);

    //std::ifstream statm("/proc/self/smaps");
    //std::cout << statm.rdbuf();
    //return instance.Func();
	return 0;
}


