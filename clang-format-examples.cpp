#define STR(x)		 #x
#define STRINGIFY(x) STR(x)
#define FILE_LINE	 __FILE__ ":" STRINGIFY(__LINE__)

libCT_IMPL::libCT_IMPL() {
	SharedCounter = msm->find<unsigned int>(CT::SharedCounterName.c_str());
	SharedCounterMutex = msm->find<boost::interprocess::interprocess_mutex>(
		CT::SharedCounterMutexName.c_str());
}

class Log : public std::ostream {
  public:
	static bool		isEnabled(LogLevel level);
	static bool		isDebugging();
	static void		setLogLevel(LogLevel level);
	static LogLevel getLogLevel();
};

#define X_ENUM_WITH_CONVERSIONS(name, enumerators)                \
	typedef enum t_name { BOOST_PP_SEQ_ENUM(enumerators) } name;  \
                                                                  \
	inline const char *ToString(name v) {                         \
		switch (v) {                                              \
			BOOST_PP_SEQ_FOR_EACH(X_ENUM_CASE, name, enumerators) \
		default:                                                  \
			return "[Unknown " BOOST_PP_STRINGIZE(name) "]";      \
		}                                                         \
	}

void Server::Quit() {
	try {
		t1->join();
	} catch (...) {
		LOG(Log::ERROR) << " Quit: Exception caught." << std::endl;
	}
	switch (a) {
	case 1:
		break;
	case 2:
		return;
	}
	if (a) {
		return;
	}
}

bool Server::IsRunning() const {
	return Running;
}

void Server::IfLambda() {
	if (a)
		return;
	else
		return;
	auto lambda2 = [](int a) {
		return a;
	};
	auto lambda3 = [](int a) {
	};
	while (true)
		continue;
}

void Server::IfLambda() {
	if (a)
		return;
	auto lambda2 = [](int a) {
		return a;
	}
}

for (auto i = 0u; i < numComponents; i++) {}

class libCT_IMPL : public org::axis::CT_adaptor, public DBus::ObjectAdaptor {
	libCT(DBus &conn): DBus::OA(conn, DBus_ServerPath.c_str()) {}
}

class Log : public std::ostream {
  public:
	X_ENUM_WITH_CONVERSIONS(LogLevel, X_LOGLEVEL_NAMES);

	std::unordered_map<enum LogLevel, int> loglevel_map = {
		{NOTICE, 5}, {INFO, 6}, {WARNING, 4}, {ERROR, 3}, {DEBUG, 7},
	};

	explicit Log() { return; };
	~Log();

	Log &writeToBuffer(LogLevel level, const std::string &fl,
					   const std::string &fmt, const std::string &s1,
					   const std::string &s2);
	Log &writeToBuffer(LogLevel level, const std::string &fl);

	static bool		isEnabled(LogLevel level);
	static bool		isDebugging();
	static void		setLogLevel(LogLevel level);
	static LogLevel getLogLevel();

  private:
	std::stringbuf		 m_Buffer;
	static enum LogLevel s_LevelOfPrint;
};

#if FOO
#if BAR
#include <foo>
#endif
#endif

void Server::IfLambda() {
	int					i = (int)a;
	template <int> void foo(); // Comment 1
	vector<int>{1, 2, 3}; // Comment 2
	for (auto v : values) {}
	var arr = [ 1, 2, 3 ];
}

class Log : public std::ostream {
	  public : sta
	  tic bool isEnabled(LogLevel level);
	    static bool        isDebugging();
	    static void        setLogLevel(LogLevel level);
	    static LogLevel getLogLevel();
};
