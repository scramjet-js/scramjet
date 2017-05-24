cmake_minimum_required (VERSION 2.6)
include_directories("sjtm")
add_subdirectory(sjtm)
add_library(sjt $<TARGET_OBJECTS:sjtm>)
target_link_libraries(sjt
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/bde/build/groups/bdl/libbdl.a)
target_link_libraries(sjt
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/bde/build/groups/bsl/libbsl.a)
target_link_libraries(sjt
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/bde/build/thirdparty/decnumber/libdecnumber.a)
target_link_libraries(sjt
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/bde/build/thirdparty/inteldfp/libinteldfp.a)

