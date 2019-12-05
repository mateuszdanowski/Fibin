#pragma once

#include <type_traits>
#include <typeinfo>
#include <cstdio>
#include <cstdint>

// A type for variable names:
using var_t = uint64_t;


// A type for Fibonacci numbers:
template <unsigned int n>
struct Fib
{
    enum { value = Fib<n-1>::value + Fib<n-2>::value };
};

template <>
struct Fib<0>
{
    enum { value = 0 };
};

template <>
struct Fib<1>
{
    enum { value = 1 };
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
struct EnvLookup<Var, EmptyEnv> {};

template <var_t Var, typename Value, typename Env>
struct EnvLookup<Var, Binding<Var, Value, Env>>
{
    using result = Value;
};

template <var_t Var, var_t Var2, typename Value2, typename Env>
struct EnvLookup<Var, Binding<Var2, Value2, Env>>
{
    using result = typename EnvLookup<Var, Env>::result;
};


// Values:
template <typename Lambda, typename Env>
struct Closure {};

struct True {};
struct False {};


// Eval:
template <typename Expr, typename Env, typename ValueType>
struct Eval {};

// Apply:
template <typename Proc, typename Value, typename ValueType>
struct Apply {};

// Literals:
template <typename T, typename Env, typename ValueType>
struct Eval<Lit<T>, Env, ValueType>
{
    using result = T;
};

// Variable references:
template <var_t Var, typename Env, typename ValueType>
struct Eval<Ref<Var>, Env, ValueType>
{
    using result = typename EnvLookup<Var, Env>::result;
};

// Lambdas:
template <var_t Var, typename Body, typename Env, typename ValueType>
struct Eval<Lambda<Var, Body>, Env, ValueType>
{
    using result = Closure<Lambda<Var, Body>, Env>;
};

// Invokes:
template <typename Fun, typename Arg, typename Env, typename ValueType>
struct Eval<Invoke<Fun, Arg>, Env, ValueType>
{
    using result = typename Apply<
            typename Eval<Fun, Env, ValueType>::result,
            typename Eval<Arg, Env, ValueType>::result,
            ValueType>::result;
};

// Branch true:
template <typename Then, typename Else, typename Env, typename ValueType>
struct Eval<If<True, Then, Else>, Env, ValueType>
{
    using result = typename Eval<Then, Env, ValueType>::result;
};

// Branch false:
template <typename Then, typename Else, typename Env, typename ValueType>
struct Eval<If<False, Then, Else>, Env, ValueType>
{
    using result = typename Eval<Else, Env, ValueType>::result;
};

// Evaluate the condition:
template <typename Condition, typename Then, typename Else,
          typename Env, typename ValueType>
struct Eval<If<Condition, Then, Else>, Env, ValueType>
{
    using result = typename Eval<
            If<typename Eval<Condition, Env, ValueType>::result, Then, Else>,
            Env,
            ValueType>::result;
};

// Let:
template <var_t Var, typename Value, typename Expression,
          typename Env, typename ValueType>
struct Eval<Let<Var, Value, Expression>, Env, ValueType>
{
    using result = typename Eval<
            Expression,
            Binding<Var, typename Eval<Value, Env, ValueType>::result, Env>,
            ValueType>::result;
};


// Transition to the body of the lambda term inside the closure:
template <var_t Name, typename Body, typename Env, typename Value,
          typename ValueType>
struct Apply<Closure<Lambda<Name, Body>, Env>, Value, ValueType>
{
    using result = typename Eval<
            Body,
            Binding<Name, Value, Env>,
            ValueType>::result;
} ;


// Eq
template <typename LHS, typename RHS>
struct Eq {};

template <bool logicalValue>
struct EqHelp {};

template <>
struct EqHelp<true> {
    using result = True;
};

template <>
struct EqHelp<false> {
    using result = False;
};

template <typename LHS, typename RHS, typename Env, typename ValueType>
struct Eval<Eq<LHS, RHS>, Env, ValueType>
{
    using result = typename EqHelp<
            static_cast<ValueType>
            (Eval<LHS, Env, ValueType>::result::value) ==
            static_cast<ValueType>
            (Eval<RHS, Env, ValueType>::result::value)
            >::result;
};


// Addition:
template <typename Term1, typename Term2, typename... Terms>
struct Sum {};

template <typename Arg>
using Inc1 = Sum<Arg, Lit<Fib<1>>>;

template <typename Arg>
using Inc10 = Sum<Arg, Lit<Fib<10>>>;

template <template <typename Term1, typename Term2, typename... Terms> class Sum,
          typename Env, typename Term1, typename Term2, typename... Terms,
          typename ValueType>
struct Eval<Sum<Term1, Term2, Terms...>, Env, ValueType>
{
    struct result
    {
        enum {
            value = Eval<Term1, Env, ValueType>::result::value +
                    Eval<Sum<Term2, Terms...>, Env, ValueType>::result::value
        };
    };
};

template <template <typename Term1, typename Term2, typename... Terms> class Sum,
          typename Env, typename Term1, typename Term2, typename ValueType>
struct Eval<Sum<Term1, Term2>, Env, ValueType>
{
    struct result
    {
        enum {
            value = Eval<Term1, Env, ValueType>::result::value +
                    Eval<Term2, Env, ValueType>::result::value
        };
    };
};


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
        return static_cast<ValueType>(Eval<Expr, EmptyEnv, ValueType>::result::value);
    }
};


namespace details {

    constexpr char toLower(const char c) {
        return ('A' <= c && c <= 'Z') ? (c + ('a' - 'A')) : c;
    }

    constexpr bool isCorrectChar(const char c) {
        return ('0' <= c && c <= '9') ||
               ('a' <= c && c <= 'z') ||
               ('A' <= c && c <= 'Z');
    }

    constexpr size_t getLen(const char *c) {
        size_t len = 0;
        while (c[len] != '\0') {
            len++;
        }
        return len;
    }
}

// Variables:
constexpr var_t Var(const char* name) {
    if (name == nullptr) {
        throw "IncorrectVarName";
    }
    size_t len = details::getLen(name);
    if (len < 1 || 6 < len) {
        throw "IncorrectVarName";
    }

    var_t hash = 1;
    for (int i = 0; i < len; i++) {
        if (!details::isCorrectChar(name[i])) {
            throw "IncorrectVarName";
        }
        hash = hash * 137 + details::toLower(name[i]);
    }
    return hash;
}