#include <iostream>

class A {
	public:
		std::string m_Name = "A";
	public:
		void Print() {
			std::cout << "Print(): " << m_Name << std::endl;
		}
};

class B {
	private:
		A m_A;
		A& m_A_Ref;
	public:
		B(A& a) : m_A_Ref(a) {};

		A& GetA_Ref() {
			return m_A;
		}

		A* GetA_Point() {
			return &m_A;
		}

		A& GetA_Func() {
/*
			B b;
			return b.GetA_Ref();
*/
			return m_A_Ref;
		}
};



int main(int argc, char *argv[]) {
/*
	B* b1 = new B();
	A& a1 = b1->GetA_Ref();
	A* a2 = b1->GetA_Point();
*/

	{
		A a;
	}
	B b(a);
	A& aref = b.GetA_Func();


/*
	A& a3 = b2.GetA_Func();
	a3.Print();
*/

//	std::cout << "Test: " << a3.m_Name << std::endl;
//	delete b1;
}
