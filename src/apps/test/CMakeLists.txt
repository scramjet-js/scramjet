cmake_minimum_required (VERSION 2.8)

project (tester)
add_executable(tester main.cpp)

target_link_libraries(tester LLVMCore)
