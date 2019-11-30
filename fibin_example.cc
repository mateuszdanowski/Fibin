#include "fibin.h"
#include <cstdint>
#include <iostream>
using namespace std;


int main() {

    int n = Fib<1>::value;
    int m = Fib<20>::value;
    cout << n << " " << m << endl;

    cout << Eval<Lit<Fib<10>>>::result::value << endl;

    Fibin<const char*>;
    Fibin<bool>;
    Fibin<u_int8_t>;
    //Fibin<const char*>::eval<Lit<Fib<0>>>();
    int res = Fibin<int>::eval<
            If<
            Lit<True>,
            Lit<Fib<1>>,
            Lit<Fib<2>>>
            >();
    cout << res << endl;

//    // Testing: lambda(x) {x + (Fib(1) + Fib(10)) + Fib(2)}(Fib(3))
//    // Fib(0) = 0, Fib(1) = 1, Fib(2) = 1, Fib(3) = 2, Fib(10) = 55
//    static_assert(59 == Fibin<uint64_t>::eval<Invoke<Lambda<Var("x"),
//                  Sum<Ref<Var("x")>, Inc10<Lit<Fib<1>>>, Lit<Fib<2>>>>, Lit<Fib<3>>>>());
//
//    // Testing: if False then Fib(0) else Fib(1)
//    static_assert(1 == Fibin<uint8_t>::eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1>>>>());
//
//    // Testing: let z = Fib(0) in {z + Fib(1)}
//    static_assert(1 == Fibin<int16_t>::eval<Let<Var("z"), Lit<Fib<0>>, Inc1<Ref<Var("Z")>>>>());
//
//    // Prints out to std::cout: "Fibin doesn't support: PKc"
//    Fibin<const char*>::eval<Lit<Fib<0>>>();
//
//    std::cout << "Fibin works fine!" << std::endl;
}

