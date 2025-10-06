#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "Fine-Grained-Locking-Common.hpp"

namespace bip = boost::interprocess;

const char *SOCKET_PATH = "/tmp/server_socket";

int main() {
    std::string shm_name = "client_sync_" + std::to_string(getpid());
    bip::shared_memory_object::remove(shm_name.c_str());
    bip::managed_shared_memory shm(bip::create_only, shm_name.c_str(), 4096);
    ClientSync* cs = shm.construct<ClientSync>("sync")();
	strncpy(cs->shmName, shm_name.c_str(), shm_name.size() + 1);

    // Announce shm name to server
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    connect(sock, (sockaddr*)&addr, sizeof(addr));
    write(sock, shm_name.c_str(), shm_name.size()+1);
    close(sock);

    // Simulate reading server buffer with a sharable lock
    ClientSync *sync = shm.find<ClientSync>("sync").first;
	while (true) {
		{
			bip::sharable_lock<bip::interprocess_upgradable_mutex> lock(sync->mutex);
			std::cout << "Client " << getpid() << " reading shared data...";
			sleep(1);
		}
		std::cout << " done" << std::endl;
		sleep(1);
	}
}
