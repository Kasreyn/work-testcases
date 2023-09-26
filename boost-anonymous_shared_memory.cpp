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

/*
        boost::interprocess::managed_shared_memory segmentManager(boost::interprocess::create_only, "Test", segmentSize, segmentAddress, 0660);
        boost::interprocess::interprocess_mutex* mutex =
            segmentManager.construct<boost::interprocess::interprocess_mutex>("MyMutex")();
*/

    } catch (const boost::interprocess::interprocess_exception& e) {
        // Handle exceptions as needed
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

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
