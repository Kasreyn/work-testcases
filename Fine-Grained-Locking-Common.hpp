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

struct ClientSync {
	char shmName[100];
	bip::interprocess_upgradable_mutex mutex;
};

class ClientXSI {
  private:
	bip::xsi_shared_memory xsm;
	bip::mapped_region mr;
	bip::managed_external_buffer meb;
	unsigned int timeoutCounter;

  public:
	ClientSync* client_sync;

  public:
	ClientXSI()
		: xsm(bip::open_or_create, bip::xsi_key{key_t(0)}, 4096, 0600), mr(xsm, bip::read_write),
		  meb(bip::create_only, mr.get_address(), mr.get_size()) {
		client_sync	   = meb.construct<ClientSync>("sync")();
		timeoutCounter = 0;
	}

	~ClientXSI() {
		bip::xsi_shared_memory::remove(xsm.get_shmid());
	}

	unsigned int GetKey() {
		return xsm.get_shmid();
	}

	bool TimeoutExpired() {
		return (++timeoutCounter > 4);
	}

	void Reset() {
		timeoutCounter = 0;
	}

	bip::interprocess_upgradable_mutex& GetMutex() {
		return client_sync->mutex;
	}
};
