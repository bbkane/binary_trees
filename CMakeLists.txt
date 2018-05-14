cmake_minimum_required(VERSION 3.3)

project (binary_trees)

add_executable(binary_trees main.cpp)

list(APPEND my_compile_options  -std=c++14 -Wextra -Wall -Weverything -pedantic -Wno-c++98-compat -Wno-missing-prototypes)

list(APPEND my_compile_options -g)  # for valgrind
# it's warning me aout BinaryTree byte-alignment, but I can't do anything about
# that right now
list(APPEND my_compile_options -Wno-padded)


target_compile_options(binary_trees PUBLIC "${my_compile_options}")

add_custom_target(valgrind
    COMMAND valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes $<TARGET_FILE:binary_trees>)
