#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <sstream>

struct FloatTest {
	float m_Float;

/*
	FloatTest(float value) : m_Float(value) {
	}
*/

	std::string ToString() const {
		std::stringstream ss;
		ss << m_Float;
		return ss.str();
	}
};

std::ostream& operator<<(std::ostream& os, const FloatTest& p) {
	return os << p.m_Float;
}

int main() {
	std::vector<FloatTest> coordAlternatives = { {1.1}, {2.2}, {3.3} };
	std::vector<FloatTest> coordAlternatives2;
/*
	coordAlternatives.push_back(FloatTest(1.1));
	coordAlternatives.push_back(FloatTest(2.2));
	coordAlternatives.push_back(FloatTest(3.3));
*/
//	= { FloatTest(1.1), FloatTest(2.2), FloatTest(3.3) };
//	std::cout << FloatTest(2.2) << std::endl;
	auto inserter = std::back_insert_iterator<decltype(coordAlternatives)>(coordAlternatives2);
	std::stringstream ss;

/*
	for (auto& coord : coordAlternatives) {
	}
*/

/*
	std::copy_if(coordAlternatives.begin(), coordAlternatives.end(), inserter, [&ss](const FloatTest& f) {
//			ss << f.ToString() << ',';
			ss << f;
			if (true) {
				ss << ',';
			}
			return true;
		}
	);
*/

	int index = 0;
    std::for_each(coordAlternatives.begin(), coordAlternatives.end(), 
		  [&index, size = coordAlternatives.size()](const FloatTest& ft) {
			  std::cout << ft.m_Float;
			  if (++index < size) {
				  std::cout << ", ";
			  }
		  }
	);

	std::cout << ss.str() << std::endl;
}
