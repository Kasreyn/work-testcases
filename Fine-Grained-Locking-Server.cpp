#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <vector>

#include "Fine-Grained-Locking-Common.hpp"

namespace bip = boost::interprocess;

const char *SOCKET_PATH = "/tmp/server_socket";

int main() {
    unlink(SOCKET_PATH);
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    bind(sock, (sockaddr*)&addr, sizeof(addr));
    listen(sock, 5);

    std::vector<std::string> client_shms;

    for (int i = 0; i < 2; i++) {
        int client_fd = accept(sock, nullptr, nullptr);
        char buf[128];
        read(client_fd, buf, sizeof(buf));
        client_shms.emplace_back(buf);
        std::cout << "Server: got client shm name " << buf << std::endl;
        close(client_fd);
    }

	std::vector<bip::managed_shared_memory> regions;
    std::vector<bip::offset_ptr<ClientSync>> syncs;
    for (auto &name : client_shms) {
		bip::managed_shared_memory& region = regions.emplace_back(bip::open_only, name.c_str());
        syncs.push_back(region.find<ClientSync>("sync").first);
    }

	while (true) {
		std::cout << "Server: taking exclusive lock to update shared buffer..." << std::endl;

		std::vector<bip::scoped_lock<bip::interprocess_upgradable_mutex>> locks;
		for (bip::offset_ptr<ClientSync> sync : syncs) {
			locks.emplace_back(sync->mutex);
			std::cout << "Locked client's mutex in /dev/shm/" << sync->shmName << std::endl;
		}

		std::cout << "Server: Doing work (simulated)." << std::endl;
		sleep(5);

		std::cout << "Server: Work complete (simulate server doing other things)." << std::endl;
		locks.clear();
		sleep(5);
	}

    close(sock);
    unlink(SOCKET_PATH);
}
