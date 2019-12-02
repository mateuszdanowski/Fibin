#pragma once

#include <type_traits>
#include <typeinfo>
#include <cstdio>

// A type for variable names:
using var_t = uint64_t;


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


// Syntax:
template <typename T>
struct Lit {};

template <var_t Var>
struct Ref {};

template <var_t Var, typename Value, typename Expression>
struct Let {};

template <typename Fun, typename Param>
struct Invoke {} ;

template <typename Condition, typename Then, typename Else>
struct If {};

template <var_t Var, typename Body>
struct Lambda {};


// Environments:
struct EmptyEnv;

template <var_t Var, typename Value, typename Env>
struct Binding {};

template <var_t Var, typename Env>
struct EnvLookup {};

template <var_t Var>
struct EnvLookup <Var, EmptyEnv> {};

template <var_t Var, typename Value, typename Env>
struct EnvLookup <Var, Binding<Var, Value, Env>>
{
    using result = Value;
};

template <var_t Var, var_t Var2, typename Value2, typename Env>
struct EnvLookup <Var, Binding<Var2, Value2, Env>>
{
    using result = typename EnvLookup<Var, Env>::result;
};


// Values:
template <typename Lambda, typename Env>
struct Closure {} ;

struct True {};
struct False {};


// Eval:
template <typename Expr, typename Env>
struct Eval {} ;

template <typename Proc, typename Value>
struct Apply {} ;

// Literals:
template <typename T, typename Env>
struct Eval <Lit<T>, Env>
{
    using result = T;
};

// Variable references:
template <var_t Var, typename Env>
struct Eval <Ref<Var>, Env>
{
    using result = typename EnvLookup<Var, Env>::result;
};

// Lambdas:
template <var_t Var, typename Body, typename Env>
struct Eval <Lambda<Var,Body>, Env>
{
    using result = Closure<Lambda<Var, Body>, Env>;
};

template <typename Fun, typename Arg, typename Env>
struct Eval<Invoke<Fun, Arg> , Env> {
    typename Apply<typename Eval<Fun,Env> :: result ,
            typename Eval<Arg,Env> :: result > :: result
    typedef result ;
} ;


// Branch true:
template <typename Then, typename Else, typename Env>
struct Eval<If<True, Then, Else>, Env> {
    using result = typename Eval<Then,Env> :: result;
} ;

// Branch false:
template <typename Then, typename Else, typename Env>
struct Eval<If<False, Then, Else>, Env> {
    using result = typename Eval<Else,Env> :: result;
} ;

// Evaluate the condition:
template <typename Condition, typename Then, typename Else, typename Env>
struct Eval<If<Condition, Then, Else>, Env> {
    using result = typename Eval<If<typename Eval<Condition,Env> :: result, Then, Else>, Env> :: result;
} ;

// Let:
template <var_t Var, typename Value, typename Expression, typename Env>
struct Eval<Let <Var, Value, Expression>, Env> {
    using result = typename Eval <Expression, Binding <Var, typename Eval<Value, Env>::result, Env>> :: result;
};


// Transition to the body of the lambda term inside the closure:
template <var_t Name, typename Body, typename Env, typename Value>
struct Apply<Closure<Lambda<Name,Body>, Env>, Value> {
    typename Eval<Body, Binding<Name,Value,Env> > :: result
    typedef result ;
} ;


// TODO
// Addition:
template <typename Term1, typename Term2, typename... Terms>
struct Sum {

};

template <typename Arg>
using Inc1 = Sum<Arg, Lit<Fib<1>>>;

template <typename Arg>
using Inc10 = Sum<Arg, Lit<Fib<10>>>;


// Fibin class:
template <typename ValueType, bool = std::is_integral<ValueType>::value>
struct Fibin {

    template <typename Expr>
    constexpr static void eval() {
        printf("Fibin doesn't support: %s\n", typeid(ValueType).name());
    }
};

template <typename ValueType>
struct Fibin<ValueType, true> {

    template <typename Expr>
    constexpr static ValueType eval() {
        return Eval<Expr, EmptyEnv>::result::value;
    }
};

constexpr char toLower(const char c) {
    return ('A' <= c && c <= 'Z') ? (c + ('a'-'A')) : c;
}

constexpr bool isCorrectChar(const char c) {
    return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

constexpr size_t getLen(const char* c) {
    size_t len = 0;
    while (c[len] != '\0') {
        len++;
    }
    return len;
}

constexpr int64_t Var(const char* name) {
    if (name == nullptr) {
        throw "IncorrectVarName";
    }
    size_t len = getLen(name);
    if (len < 1 || 6 < len) {
        throw "IncorrectVarName";
    }

    uint64_t hash = 1;
    for (int i = 0; i < len; i++) {
        if (!isCorrectChar(name[i])) {
            throw "IncorrectVarName";
        }
        hash = hash * 137 + toLower(name[i]);
    }
    return hash;
}