#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include <ranges>

struct FloatTest {
	float m_Float;

/*
	FloatTest(float value) : m_Float(value) {
	}
*/

	auto operator<=>(const FloatTest&) const = default;

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

    std::vector<int> myVector = {1, 2, 3, 4, 5};

/*
	int size = myVector.size();
    for (const auto [index, element] : std::views::iota(0, size) | std::views::transform([&myVector](int i) {
        return std::pair<int, int>(i, myVector[i]);
    })) {
        std::cout << "Element at index " << index << ": " << element << std::endl;
    }
*/

	auto makePairs = [&coordAlternatives](int i) {
		return std::pair<int, FloatTest>(i, coordAlternatives[i]);
	};

/*
    for (const auto [index, element] : std::views::iota(0, coordAlternatives.size()) | std::views::transform([&coordAlternatives](int i) {
        return std::pair<int, FloatTest>(i, coordAlternatives[i]);
    })) {
        std::cout << "Element at index " << index << ": " << element << std::endl;
    }

	int size = coordAlternatives.size();
    for (const auto [index, element] : std::views::iota(0, size) | std::views::transform(makePairs)) {
        std::cout << "Element at index " << index << ": " << element << std::endl;
    }
*/

	const std::vector<FloatTest>& vft = coordAlternatives;
	for (const auto& element : vft) {
		const auto it = std::find(vft.begin(), vft.end(), element);
		int index = std::distance(vft.begin(), it);
//		const auto it = std::find(coordAlternatives.begin(), coordAlternatives.end(), element);
        std::cout << "Element at index " << index << ": " << element << std::endl;
	}

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
