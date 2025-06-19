#include "name-mangling-testcase.hpp"

/*
g++ -o name-mangling-testcase-client name-mangling-testcase-client.cpp -L. -lname-mangling-testcase
nm -D name-mangling-testcase-client
                 w __cxa_finalize@GLIBC_2.2.5
                 w __gmon_start__
                 U __gxx_personality_v0@CXXABI_1.3
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
                 U __libc_start_main@GLIBC_2.34
                 U strlen@GLIBC_2.2.5
                 U _Unwind_Resume@GCC_3.0
                 U _ZN6Person5SetupEv@CTL_1.0
                 U _ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_M_dataEv@GLIBCXX_3.4.21
                 U _ZNSaIcEC1Ev@GLIBCXX_3.4
                 U _ZNSaIcED1Ev@GLIBCXX_3.4
                 U _ZNSaIcED2Ev@GLIBCXX_3.4
                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv@GLIBCXX_3.4.21
                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_capacityEm@GLIBCXX_3.4.21
                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_Alloc_hiderC1EPcRKS3_@GLIBCXX_3.4.21
00000000000014f6 W _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructIPKcEEvT_S8_St20forward_iterator_tag
                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13_M_local_dataEv@GLIBCXX_3.4.21
                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13_M_set_lengthEm@GLIBCXX_3.4.21
                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13_S_copy_charsEPcPKcS7_@GLIBCXX_3.4.21
                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_M_dataEPc@GLIBCXX_3.4.21
                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@GLIBCXX_3.4.21
                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1ERKS4_@GLIBCXX_3.4.21
                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@GLIBCXX_3.4.21
                 U _ZSt19__throw_logic_errorPKc@GLIBCXX_3.4
*/

int main() {
    Person person("John");
    person.Setup();

    return 0;
}

