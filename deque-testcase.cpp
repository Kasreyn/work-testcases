#include <iostream>
#include <queue>
#include <deque>
#include <vector>

int main()
{
    std::queue<int> q;
 
	int counter = 0;
	q.push_back(counter++);
	q.push_back(counter++);
    for(auto &i: q)    {
//		q.push_back(counter++);
        std::cout << i << std::endl;
    }
 
    return 0;
}

