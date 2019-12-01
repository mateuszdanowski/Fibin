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


// Fibin class:
template <typename ValueType>
struct Fibin {

    // Works for all ValueType (shouldn't for const char* etc)
    template <typename T>
    constexpr static ValueType eval() {
        return Eval<T>::result::value;
    }
};
