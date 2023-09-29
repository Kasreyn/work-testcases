#include <iostream>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/xsi_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>

class my_xsi_shared_memory : public boost::interprocess::xsi_shared_memory {
	public:
		my_xsi_shared_memory(const boost::interprocess::xsi_key &key) {
			boost::interprocess::permissions p;
			p.set_permissions(0440);
			this->my_open_or_create(boost::interprocess::ipcdetail::DoOpen, key, p, 0);
		}

		inline bool my_open_or_create
		   (boost::interprocess::ipcdetail::create_enum_t type, const boost::interprocess::xsi_key &key, const boost::interprocess::permissions& permissions, std::size_t size)
		{
		   int perm = (int)permissions.get_permissions();
		   perm &= 0x01FF;
		   int shmflg = perm;

		   switch(type){
			  case boost::interprocess::ipcdetail::DoOpen:
				 shmflg |= 0;
			  break;
			  case boost::interprocess::ipcdetail::DoCreate:
				 shmflg |= IPC_CREAT | IPC_EXCL;
			  break;
			  case boost::interprocess::ipcdetail::DoOpenOrCreate:
				 shmflg |= IPC_CREAT;
			  break;
			  default:
				 {
					boost::interprocess::error_info err = boost::interprocess::other_error;
					throw boost::interprocess::interprocess_exception(err);
				 }
		   }

		   int ret = ::shmget(key.get_key(), size, shmflg);
		   int shmid = ret;
		   if((type == boost::interprocess::ipcdetail::DoOpen) && (-1 != ret)){
			  //Now get the size
			  ::shmid_ds xsi_ds;
			  ret = ::shmctl(ret, IPC_STAT, &xsi_ds);
			  size = xsi_ds.shm_segsz;
		   }
		   if(-1 == ret){
			  boost::interprocess::error_info err = boost::interprocess::system_error_code();
			  throw boost::interprocess::interprocess_exception(err);
		   }

		   // m_shmid = shmid;
			swap(*this);
		   return true;
		}
};

void WriteStringToSharedMemory(const char* message) {
	boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_or_create, {"/tmp/", 1}, 10000, 0640);
	boost::interprocess::mapped_region mr(xsm, boost::interprocess::read_write);
    boost::interprocess::managed_external_buffer extBuffer(boost::interprocess::create_only, mr.get_address(), mr.get_size());

    auto sharedString = extBuffer.find_or_construct<boost::interprocess::basic_string<char>>("MyString")();
    sharedString->assign(message);
    std::cout << "String '" << message << "' written to shared memory." << std::endl;
}

void ReadStringFromSharedMemory() {
	//boost::interprocess::xsi_shared_memory xsm(boost::interprocess::open_or_create, {"/tmp/", 1}, 10000, 0640);
	my_xsi_shared_memory xsm({"/tmp/", 1});
	boost::interprocess::mapped_region mr(static_cast<boost::interprocess::xsi_shared_memory>(std::move(xsm)), boost::interprocess::read_only);
	std::cout << "ReadStringFromSharedMemory! key=" << std::endl;
    boost::interprocess::managed_external_buffer extBuffer(boost::interprocess::open_only, mr.get_address(), mr.get_size());

    auto sharedString = extBuffer.find<boost::interprocess::basic_string<char>>("MyString").first;
    if (!sharedString) {
        std::cerr << "String not found in shared memory." << std::endl;
        return;
    }

    std::cout << "String read from shared memory: " << *sharedString << std::endl;
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
