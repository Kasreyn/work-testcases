#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/containers/string.hpp>

// Define the name for the shared memory segment
const char* SharedMemoryName = "MySharedMemory";

void WriteStringToSharedMemory(const char* message) {
    // Create or open the shared memory segment using POSIX
    key_t key = ftok("/tmp", 'R'); // Replace "/tmp" with your desired path
    int shmId = shmget(key, 65536, IPC_CREAT | 0666);

    if (shmId == -1) {
        perror("shmget");
        return;
    }

    // Attach to the shared memory segment
    void* segmentAddress = shmat(shmId, nullptr, 0);

    if (segmentAddress == (void*)-1) {
        perror("shmat");
        return;
    }

    // Create a managed_external_buffer and map the shared memory into it
    boost::interprocess::managed_external_buffer extBuffer(boost::interprocess::create_only, segmentAddress, 65536);

    // Create or find a string in the shared memory
    auto sharedString = extBuffer.find_or_construct<boost::interprocess::basic_string<char>>("MyString")();

    // Write the message into the shared string
    sharedString->assign(message);

    std::cout << "String '" << message << "' written to shared memory." << std::endl;

    // Detach from the shared memory segment
    shmdt(segmentAddress);
}

void ReadStringFromSharedMemory() {
    // Open the shared memory segment using POSIX
    key_t key = ftok("/tmp", 'R'); // Replace "/tmp" with the same path used for writing
    int shmId = shmget(key, 65536, 0666);

    if (shmId == -1) {
        perror("shmget");
        return;
    }

    // Attach to the shared memory segment
    void* segmentAddress = shmat(shmId, nullptr, 0);

    if (segmentAddress == (void*)-1) {
        perror("shmat");
        return;
    }

    // Create a managed_external_buffer and map the shared memory into it
    boost::interprocess::managed_external_buffer extBuffer(boost::interprocess::open_only, segmentAddress, 65536);


    // Find the shared string in the shared memory
    auto sharedString = extBuffer.find<boost::interprocess::basic_string<char>>("MyString").first;

    if (!sharedString) {
        std::cerr << "String not found in shared memory." << std::endl;
        return;
    }

    // Print the string from shared memory
    std::cout << "String read from shared memory: " << *sharedString << std::endl;

    // Detach from the shared memory segment
    shmdt(segmentAddress);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <write|read>" << std::endl;
        return 1;
    }

    std::string option(argv[1]);

    if (option == "write") {
        // Write a string into shared memory
        WriteStringToSharedMemory("Hello, Shared Memory!");
    } else if (option == "read") {
        // Read and print the string from shared memory
        ReadStringFromSharedMemory();
    } else {
        std::cerr << "Invalid option. Use 'write' or 'read'." << std::endl;
        return 1;
    }

    return 0;
}

/*
#include <boost/interprocess/xsi_shared_memory.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
//#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>

int main() {
    try {
        boost::interprocess::xsi_key key("/tmp/", 1);
		boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_or_create, key, 10000);

        // Use standard POSIX functions to obtain the address and size of the segment
        int shmId = shmget(key.get_key(), 0, 0);
        if (shmId == -1) {
            std::cerr << "Failed to get the shared memory segment ID" << std::endl;
            return 1;
        }

        struct shmid_ds shmInfo;
        if (shmctl(shmId, IPC_STAT, &shmInfo) == -1) {
            std::cerr << "Failed to obtain segment information" << std::endl;
            return 1;
        }

		void* address = shmat(shmId, nullptr, 0);
		if (address == (void*)-1) {
            std::cerr << "Failed to get the shared memory address errno:" << errno << std::endl;
            return 1;
		}

		std::cout << shmId << std::endl;
		std::cout << std::hex << address << std::endl;
		std::cout << shmInfo.shm_segsz  << std::endl;

        const char* segmentAddress = reinterpret_cast<const char*>(address);
        std::size_t segmentSize = static_cast<std::size_t>(shmInfo.shm_segsz);

		boost::interprocess::managed_external_buffer extBuffer(boost::interprocess::create_only, address, segmentSize);
		boost::interprocess::interprocess_mutex* mutex = extBuffer.construct<boost::interprocess::interprocess_mutex>("MyMutex")();

//        boost::interprocess::managed_shared_memory segmentManager(boost::interprocess::create_only, "Test", segmentSize, segmentAddress, 0660);
//        boost::interprocess::interprocess_mutex* mutex =
//            segmentManager.construct<boost::interprocess::interprocess_mutex>("MyMutex")();

    } catch (const boost::interprocess::interprocess_exception& e) {
        // Handle exceptions as needed
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
*/

/*
#include <iostream>

#include <boost/interprocess/xsi_shared_memory.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

int main() {
	boost::interprocess::xsi_key key("/tmp", 1); 
	boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_or_create, key, 10000);
	boost::interprocess::managed_shared_memory msm(boost::interprocess::open_or_create, "Test", xsm.get_mapping_handle());

	sleep(60);

    return 0;
}
*/
