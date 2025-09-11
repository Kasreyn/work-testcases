#include <cstdint>
#include <iostream>
#include <source_location>
#include <string_view>

namespace Some::Name {

	void log(const std::string_view message,
			 const std::source_location location =
				   std::source_location::current())
	{
		std::clog << "file: "
				  << location.file_name() << '('
				  << location.line() << ':'
				  << location.column() << ") `"
				  << location.function_name() << "`: "
				  << message << '\n';
	}
 

	void hi() {
		log("Hello!");
	}
}

int main() {
	int16_t i16 = -2;
	uint32_t u32 = (static_cast<uint32_t>(i16) & ((1u << 18) - 1));
	std::cout << "test: " << u32 << std::endl;
	uint32_t n = 2;
	uint32_t d = 3;
//	double f = static_cast<double>(d) / n;
	double f = d / n;
	std::cout << "test 2: " << f << std::endl;

	int width = 0;

	d = width;

	Some::Name::hi();
}
