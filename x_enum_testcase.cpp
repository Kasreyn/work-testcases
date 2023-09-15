//g++ -o regex_testcase regex_testcase.cpp -lboost_system -lboost_regex

#include <fstream>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/preprocessor.hpp>
#include <boost/algorithm/string.hpp>

#define X_ENUM_CASE(r, data, elem)                                             \
	case LogLevel::elem:                                                       \
		str = BOOST_PP_STRINGIZE(elem);                                        \
		boost::algorithm::to_lower(str);                                       \
		return str;

#define X_ENUM_WITH_CONVERSIONS(name, enumerators)                             \
	enum class name { BOOST_PP_SEQ_ENUM(enumerators) };                        \
																			   \
	inline const std::string ToString(name v) {                                \
		std::string str;                                                       \
		switch (v) {                                                           \
			BOOST_PP_SEQ_FOR_EACH(X_ENUM_CASE, name, enumerators)              \
			default:                                                           \
				return "[Unknown " BOOST_PP_STRINGIZE(name) "]";               \
		}                                                                      \
	}

#define X_LOGLEVEL_NAMES (NOTICE)(INFO)(WARNING)(ERROR)(DEBUG)(UNKNOWN)
X_ENUM_WITH_CONVERSIONS(LogLevel, X_LOGLEVEL_NAMES);

int main(int argc, char *argv[]) {
	LogLevel l{LogLevel::WARNING};
	std::cout << "Test: " << ToString(l) << std::endl;
}
