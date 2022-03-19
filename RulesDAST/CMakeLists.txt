cmake_minimum_required(VERSION 3.20)
project(RulesDAST)

set(CMAKE_CXX_STANDARD 17)

add_executable(RulesDAST main.cpp src/RuleTable.cpp include/RuleTable.h src/Rule.cpp include/Rule.h)
