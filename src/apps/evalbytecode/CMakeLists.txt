cmake_minimum_required (VERSION 2.8)

project (evalbytecode)
add_executable(evalbytecode main.cpp)

# TODO: figure out how to auto-derive these:
target_include_directories(evalbytecode PRIVATE ../../groups/sjt/sjtd)
target_include_directories(evalbytecode PRIVATE ../../groups/sjt/sjtt)
target_include_directories(evalbytecode PRIVATE ../../groups/sjt/sjtu)

target_link_libraries(evalbytecode sjt)
