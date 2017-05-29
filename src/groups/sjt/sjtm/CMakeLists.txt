cmake_minimum_required (VERSION 2.6)
add_library(sjtm OBJECT sjtm_engine.cpp)

add_executable(sjtm_engine.t sjtm_engine.t.cpp)
target_link_libraries(sjtm_engine.t sjt)
add_test(sjtm_engine sjtm_engine.t)
