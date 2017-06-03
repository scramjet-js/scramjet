cmake_minimum_required (VERSION 2.6)
add_library(sjtm OBJECT sjtm_datumfactory.cpp)

add_executable(sjtm_datumfactory.t sjtm_datumfactory.t.cpp)
target_link_libraries(sjtm_datumfactory.t sjt)
add_test(sjtm_datumfactory sjtm_datumfactory.t)
