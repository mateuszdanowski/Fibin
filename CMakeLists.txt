cmake_minimum_required(VERSION 3.15)
project(Fibin)

set(CMAKE_CXX_STANDARD 17)

add_executable(Fibin fibin_example.cc fibin.h)

# clang -Wall -Wextra -std=c++17 -O2 -lstdc++