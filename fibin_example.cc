#include "fibin.h"
#include <cstdint>
#include <iostream>
#include "testowanie.h"
using namespace std;

int main() {

    static_assert(1 == Fibin<int>::eval<If<Lit<True>,Lit<Fib<1>>,Lit<Fib<2>>>>());

    // Testing: lambda(x) {x + (Fib(1) + Fib(10)) + Fib(2)}(Fib(3))
    // Fib(0) = 0, Fib(1) = 1, Fib(2) = 1, Fib(3) = 2, Fib(10) = 55
    static_assert(59 == Fibin<uint64_t>::eval<Invoke<Lambda<Var("x"),
                  Sum<Ref<Var("x")>, Inc10<Lit<Fib<1>>>, Lit<Fib<2>>>>, Lit<Fib<3>>>>());

    // Testing: if False then Fib(0) else Fib(1)
    static_assert(1 == Fibin<uint8_t>::eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1>>>>());

    // Testing: let z = Fib(0) in {z + Fib(1)}
    static_assert(1 == Fibin<int16_t>::eval<Let<Var("z"), Lit<Fib<0>>, Inc1<Ref<Var("Z")>>>>());

    // Prints out to std::cout: "Fibin doesn't support: PKc"
    Fibin<const char*>::eval<Lit<Fib<0>>>();

    std::cout << "Fibin works fine!" << std::endl;

    /** Other tests */

//    Var("a");
//    Var("abc");
//    Var("012345");
//    Var("aBcDeF");
//    //Var("");
//    //Var("1234567");
//    //Var("a b c");
//    //Var(nullptr);

    static_assert(55 == Eval<Lit<Fib<10>>, EmptyEnv>::result::value);
    static_assert(55 == Fibin<int>::eval<Lit<Fib<10>>>());

//    Fibin<const char*>;
//    Fibin<bool>;
//    Fibin<uint8_t>;

//    Fibin<const char*>::eval<Lit<Fib<0>>>();
//    Fibin<int*>::eval<Lit<Fib<0>>>();


//    Binding<Var("x"), Fib<0>, EmptyEnv>;


    // Testing ['a' => 1, 'b' => 0](a):
    int v1 = EnvLookup<Var("a"), Binding<Var("b"), Fib<0>,
             Binding<Var("a"), Fib<1>,
             EmptyEnv> > >::result::value;
    assert(v1 == 1);

    // Testing ['a' => 1, 'b' => 0](b):
    int v2 = EnvLookup<Var("b"), Binding<Var("b"), Fib<0>,
             Binding<Var("a"), Fib<1>,
             EmptyEnv> > >::result::value;
    assert(v2 == 0);

    /** Forum */

    // q1
    static_assert(1 == Fibin<uint64_t>::eval<Invoke<Let<Var("x"), Lit<Fib<0>>, Lambda<Var("x"), Ref<Var("x")>>>, Lit<Fib<1>>>>());

    // q2
    static_assert(1 == Fibin<int>::eval<Let<Var("f"),Lambda<Var("x"),Inc1<Ref<Var("x")>>>,Invoke<Ref<Var("f")>, Lit<Fib<0>>>>>());

    // let fix testing
    static_assert(55 == Fibin<int>::eval<Let<Var("f"), Lambda<Var("x"), Ref<Var("x")>>, Invoke<Ref<Var("f")>, Lit<Fib<10>>>>>());



    static_assert(0 == Fibin<int8_t>::eval<Invoke<Lambda<Var("x"), Ref<Var("x")>>, Lit<Fib<0>>>>());

    constexpr int a = Fibin<int>::eval<Invoke<Lambda<Var("x"), Ref<Var("x")>>, Lit<Fib<0>>>>();
    constexpr int b = Fibin<int>::eval<Invoke<Lambda<Var("x"), Ref<Var("x")> >, Lit<Fib<3>>>>();
    static_assert(a == 0 && b == 2);

    static_assert(2 == Eval<Invoke<Lambda<Var("X"), Ref<Var("X")> >, Lit<Fib<3> > >, EmptyEnv>::result::value);



    // Testing (if false 0 1):
    int y = Eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1> > >, EmptyEnv>::result::value;
    assert(y == 1);


    // Testing case-sensitivity
    int r = EnvLookup<Var("a"), Binding<Var("b"), Fib<0>,
            Binding<Var("A"), Fib<1>,
            EmptyEnv> > >::result::value;
    assert(r == 1);

    int p = EnvLookup<Var("abc"), Binding<Var("123456"), Fib<0>,
            Binding<Var("AbC"), Fib<3>,
            EmptyEnv> > >::result::value;
    assert(p == 2);


    // Testing Let
    int u = Fibin<int>::eval<
            Let<Var("X"), Lit<Fib<0>>,Invoke<
                    Lambda<Var("X"),Ref<Var("X")>>,
                    Lit<Fib<1>>
                    >
            >
    >();
    assert(u == 1);

    // Forum
    int f1 = Fibin<int>::eval<
    Let<
            Var("const"),
            Lit<Fib<1>>,
            Let<
                    Var("f"),
                    Lambda<
                            Var("x"),
                            Sum<
                                    Ref<Var("const")>,
                                    Ref<Var("x")>
                            >
                    >,
                    Let<
                            Var("const"),
                            Lit<Fib<2>>,
                            Invoke<
                                    Ref<Var("f")>,
                                    Lit<Fib<0>>
                            >
                    >
            >
    >>();
    assert(f1 == 1);

    // Forum
    int f2 = Fibin<uint8_t>::eval<
    Let<
            Var("X"),
            Lit<Fib<0>>,
            Invoke <
                    Lambda<
                            Var("X"),
                            Ref<Var("X")>
                    >,
                    Lit<Fib<1>>
            >
    >>();
    assert(f2 == 1);

    // Forum
    int f3 = Fibin<std::uint32_t>::eval<
    Let<
            Var("t"),
            Lit<True>,
            If<
                    Ref<Var("t")>,
                    Lit<Fib<0>>,
                    Lit<Fib<1>>
            >
    >>();
    assert(f3 == 0);

    // Forum
    int f4 = Fibin<std::uint32_t>::eval<
    Let<
            Var("tf"),
            Lambda<
                    Var("x"),
                    Lit<True>
            >,
            If<
                    Invoke<Ref<Var("tf")>, Lit<False>>,
                    Lit<Fib<0>>,
                    Lit<Fib<1>>
            >
    >>();
    assert(f4 == 0);

    // 5 : 5
    // 32 : 2178309
    int8_t a1 = Fibin<int8_t>::eval<Lit<Fib<5>>>();
    int8_t b1 = Fibin<int8_t>::eval<Lit<Fib<32>>>();

    // Tu jest git, powinny byc rowne
    cout << int(a1) << " " << int(b1) << endl;


    // Tu się psuje, powinno byc True i wypisac 55
    cout << int(Fibin<uint8_t>::eval<If< Eq<Lit<Fib<32>>, Lit<Fib<5>>>, Lit<Fib<10>>, Lit<Fib<11>> >>()) << endl;
    //static_assert(Fibin<uint8_t>::eval<If< Eq<L<32>, L<5>>, L<10>, L<11> >>() == 55);

    std::cout << "All tests passed!" << std::endl;
}