#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/xsi_shared_memory.hpp>

void WriteStringToSharedMemory(const char* message) {
	boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_or_create, {"/tmp/", 1}, 10000);
    boost::interprocess::managed_external_buffer extBuffer(boost::interprocess::open_only, (void*)(xsm.get_mapping_handle().handle), 10000);

    auto sharedString = extBuffer.find_or_construct<boost::interprocess::basic_string<char>>("MyString")();
    sharedString->assign(message);
    std::cout << "String '" << message << "' written to shared memory." << std::endl;

    shmdt(reinterpret_cast<void*>(xsm.get_mapping_handle().handle));
}

void ReadStringFromSharedMemory() {
	boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_only, {"/tmp/", 1});
    boost::interprocess::managed_external_buffer extBuffer(boost::interprocess::open_only, reinterpret_cast<void*>(xsm.get_mapping_handle().handle), 10000);

    auto sharedString = extBuffer.find<boost::interprocess::basic_string<char>>("MyString").first;
    if (!sharedString) {
        std::cerr << "String not found in shared memory." << std::endl;
        return;
    }
    std::cout << "String read from shared memory: " << *sharedString << std::endl;

    shmdt(reinterpret_cast<void*>(xsm.get_mapping_handle().handle));
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <write|read>" << std::endl;
        return 1;
    }

    std::string option(argv[1]);
    if (option == "write") {
        WriteStringToSharedMemory("Hello, Shared Memory!");
    } else if (option == "read") {
        ReadStringFromSharedMemory();
    } else {
        std::cerr << "Invalid option. Use 'write' or 'read'." << std::endl;
        return 1;
    }

    return 0;
}
