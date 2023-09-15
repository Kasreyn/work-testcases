#include <iostream>
#include <eigen3/Eigen/Dense>
 
int main()
{
   Eigen::Matrix4f A;
   Eigen::Vector4f b;
   A << 1,500,900,450000, 1,599,900,539100, 1,599,999,598401, 1,500,999,499500;
   b << 454,558,598,499;
   std::cout << "Here is the matrix A:\n" << A << std::endl;
   std::cout << "Here is the vector b:\n" << b << std::endl;
   Eigen::Vector4f x = A.partialPivLu().solve(b);
   std::cout << "The solution is:\n" << x << std::endl;
}
