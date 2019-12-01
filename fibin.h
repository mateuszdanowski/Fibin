#pragma once

#include <type_traits>
#include <typeinfo>
#include <cstdio>

// A type for Fibonacci numbers:
template <unsigned int n>
struct Fib
{
    static constexpr unsigned int value = Fib<n-1>::value + Fib<n-2>::value;
};

template<>
struct Fib<0>
{
    static const unsigned int value = 0;
};

template<>
struct Fib<1>
{
    static const unsigned int value = 1;
};


// Booleans:
struct True {};
struct False {};


template <typename Condition, typename Then, typename Else>
struct If {};

template <typename T>
struct Lit {};

template <typename Expr>
struct Eval {};

// Literals evaluate to themselves:
template <typename ValueType>
struct Eval <Lit<ValueType>>
{
    typedef ValueType result;
};


// If true:
template <typename Then, typename Else>
struct Eval<If<True, Then, Else>> {
    typedef typename Eval<Then>::result result;
};

// If false:
template <typename Then, typename Else>
struct Eval<If<False, Then, Else>> {
    typedef typename Eval<Else>::result result;
};

// Evaluate the condition:
template <typename Condition, typename Then, typename Else>
struct Eval<If<Condition, Then, Else>> {
    typedef typename Eval<
            If<typename Eval<Condition>::result, Then, Else>
            >::result
    result;
};

template <typename Term1, typename Term2, typename... Terms>
struct Sum {};

template <typename Arg>
using Inc1 = Sum<Arg, Lit<Fib<1>>>;

template <typename Arg>
using Inc10 = Sum<Arg, Lit<Fib<10>>>;


// Fibin class:
template <typename ValueType, bool = std::is_integral<ValueType>::value>
struct Fibin {

    // Works for all ValueType (shouldn't for const char* etc)
    template <typename Expr>
    constexpr static void eval() {
        printf("Fibin doesn't support: %s\n", typeid(ValueType).name());
    }
};

template <typename ValueType>
struct Fibin<ValueType, true> {

    template <typename Expr>
    constexpr static ValueType eval() {
        return Eval<Expr>::result::value;
    }
};
