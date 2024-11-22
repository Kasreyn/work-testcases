#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/detail/os_file_functions.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/xsi_key.hpp>
#include <boost/interprocess/xsi_shared_memory.hpp>
#include <iostream>
#include <string>
#include <sys/types.h>

// To include boost headers into tags, run: ctags -R . /usr/include/*

int WriteStringToSharedMemory(const std::string& message) {
	boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_or_create,
											   boost::interprocess::xsi_key{key_t(0)}, 10000, 0640);
	boost::interprocess::mapped_region mr(xsm, boost::interprocess::read_write);
	boost::interprocess::managed_external_buffer extBuffer(boost::interprocess::create_only, mr.get_address(),
														   mr.get_size());

	auto sharedString = extBuffer.find_or_construct<boost::interprocess::basic_string<char>>("MyString")();
	sharedString->assign(message.c_str());
	std::cout << "String '" << message << "' written to shared memory (key " << xsm.get_shmid() << ")."
			  << std::endl;
	return xsm.get_shmid();
}

void ReadStringFromSharedMemory(int key) {
	boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_only, key);
	boost::interprocess::mapped_region mr(xsm, boost::interprocess::read_only);
	boost::interprocess::managed_external_buffer extBuffer(boost::interprocess::open_only, mr.get_address(),
														   mr.get_size());

	auto sharedString = extBuffer.find<boost::interprocess::basic_string<char>>("MyString").first;
	if (!sharedString) {
		std::cerr << "String not found in shared memory." << std::endl;
		return;
	}

	std::cout << "String read from shared memory (key " << xsm.get_shmid() << "): " << *sharedString
			  << std::endl;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <write>" << std::endl;
		return 1;
	}

	std::string option(argv[1]);
	if (option == "write") {
		int key = WriteStringToSharedMemory("Hello, Shared Memory!");
		ReadStringFromSharedMemory(key);
		boost::interprocess::xsi_shared_memory::remove(key);
	}
	else {
		std::cerr << "Invalid option. Use 'write' or 'read'." << std::endl;
		return 1;
	}

	return 0;
}
