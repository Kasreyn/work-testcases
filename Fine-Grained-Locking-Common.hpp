#pragma once
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <string>

namespace bip = boost::interprocess;

// In each client's private shm
struct ClientSync {
	char shmName[100];
    bip::interprocess_upgradable_mutex mutex;
};
