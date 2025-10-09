#pragma once
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/xsi_key.hpp>
#include <boost/interprocess/xsi_shared_memory.hpp>
#include <sys/types.h>

namespace bip = boost::interprocess;

// In each client's private shm
struct ClientSync {
	char shmName[100];
	bip::interprocess_upgradable_mutex mutex;
};

class ClientXSI {
  private:
	bip::xsi_shared_memory shm;
	bip::mapped_region region;
	bip::managed_external_buffer managed_buffer;

  public:
	ClientXSI()
		: shm(bip::open_or_create, bip::xsi_key{key_t(0)}, 4096, 0640), region(shm, bip::read_write),
		  managed_buffer(bip::create_only, region.get_address(), region.get_size()) {
		managed_buffer.construct<ClientSync>("sync")();
	}

	~ClientXSI() {
		bip::xsi_shared_memory::remove(shm.get_shmid());
	}

	unsigned int GetKey() {
		return shm.get_shmid();
	}
};
