#include <ostream>
#include <type_traits>
#include <iostream>

struct InnerConstMemberFunction {
	const int x;
	void foo() {}
};

struct InnerCtor {
	int x;
	InnerCtor() {}
};

struct Inner {
	int x;
};

struct Outer1 {
    InnerConstMemberFunction inner;
};

struct Outer2 {
    InnerCtor inner;
};

struct Outer3 : Inner {
    Inner inner;
};

int main(int argc, char* argv[]) {
    // std::is_trivial equals std::is_trivially_copyable & std::is_trivially_default_constructible
	std::cout << std::boolalpha 
			  << "Outer1 is                    trivially copy-assignable? "
			  << std::is_trivially_copy_assignable<Outer1>::value << std::endl
			  << "Outer2 is                    trivially copy-assignable? "
			  << std::is_trivially_copy_assignable<Outer2>::value << std::endl
			  << "Outer3 is                    trivially copy-assignable? "
			  << std::is_trivially_copy_assignable<Outer3>::value << std::endl
			  << "InnerConstMemberFunction is  trivially copy-assignable? "
			  << std::is_trivially_copy_assignable<InnerConstMemberFunction>::value << std::endl
			  << "InnerCtor is                 trivially copy-assignable? "
			  << std::is_trivially_copy_assignable<InnerCtor>::value << std::endl
			  << "Inner is                     trivially copy-assignable? "
			  << std::is_trivially_copy_assignable<Inner>::value << std::endl
			  << "Outer1 is                    trivial?                   " // underconstrained!
			  << std::is_trivial<Outer1>::value << std::endl
			  << "Outer2 is                    trivial?                   "
			  << std::is_trivial<Outer2>::value << std::endl
			  << "Outer3 is                    trivial?                   "
			  << std::is_trivial<Outer3>::value << std::endl
			  << "Outer1 is                    standard-layout?           "
			  << std::is_standard_layout<Outer1>::value << std::endl
			  << "Outer2 is                    standard-layout?           "
			  << std::is_standard_layout<Outer2>::value << std::endl
			  << "Outer3 is                    standard-layout?           "
			  << std::is_standard_layout<Outer3>::value << std::endl
			  << "Outer1 is                    trivially copyable?        "
			  << std::is_trivially_copyable<Outer1>::value << std::endl
			  << "Outer2 is                    trivially copyable?        "
			  << std::is_trivially_copyable<Outer2>::value << std::endl
			  << "Outer3 is                    trivially copyable?        "
			  << std::is_trivially_copyable<Outer3>::value << std::endl
			  << "Outer1 is                    pod?                       "
			  << std::is_pod<Outer1>::value << std::endl
			  << "Outer2 is                    pod?                       "
			  << std::is_pod<Outer2>::value << std::endl
			  << "Outer3 is                    pod?                       "
			  << std::is_pod<Outer3>::value << std::endl;
}

/*
Outer1 is                    trivially copy-assignable? false
Outer2 is                    trivially copy-assignable? true
Outer3 is                    trivially copy-assignable? true
InnerConstMemberFunction is  trivially copy-assignable? false
InnerCtor is                 trivially copy-assignable? true
Inner is                     trivially copy-assignable? true
Outer1 is                    trivial?                   true
Outer2 is                    trivial?                   false
Outer3 is                    trivial?                   true
Outer1 is                    standard-layout?           true
Outer2 is                    standard-layout?           true
Outer3 is                    standard-layout?           false
Outer1 is                    trivially copyable?        true
Outer2 is                    trivially copyable?        true
Outer3 is                    trivially copyable?        true
Outer1 is                    pod?                       true
Outer2 is                    pod?                       false
Outer3 is                    pod?                       false
*/
