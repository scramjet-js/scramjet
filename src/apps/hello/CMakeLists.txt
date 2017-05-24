cmake_minimum_required (VERSION 2.6)

include_directories("../../groups/sjt/sjtm")

project (hello)
add_executable(hello main.cpp)

target_link_libraries(hello sjt)
