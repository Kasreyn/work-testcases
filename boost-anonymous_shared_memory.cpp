#include <iostream>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/xsi_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>

void WriteStringToSharedMemory(const char* message) {
	boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_or_create, {"/tmp/", 2}, 10000, 0640);
	boost::interprocess::mapped_region mr(xsm, boost::interprocess::read_write);
    boost::interprocess::managed_external_buffer extBuffer(boost::interprocess::create_only, mr.get_address(), mr.get_size());

    auto sharedString = extBuffer.find_or_construct<boost::interprocess::basic_string<char>>("MyString")();
    sharedString->assign(message);
    std::cout << "String '" << message << "' written to shared memory (key " << xsm.get_shmid() << ")." << std::endl;
}

void ReadStringFromSharedMemory() {
	boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_or_create, {"/tmp/", 2}, 10000, 0440);
	boost::interprocess::mapped_region mr(xsm, boost::interprocess::read_only);
    boost::interprocess::managed_external_buffer extBuffer(boost::interprocess::open_only, mr.get_address(), mr.get_size());

    auto sharedString = extBuffer.find<boost::interprocess::basic_string<char>>("MyString").first;
    if (!sharedString) {
        std::cerr << "String not found in shared memory." << std::endl;
        return;
    }

    std::cout << "String read from shared memory (key " << xsm.get_shmid() << "): " << *sharedString << std::endl;
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
