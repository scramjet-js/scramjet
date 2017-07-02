cmake_minimum_required (VERSION 2.6)
include_directories("sjtd")
include_directories("sjtt")
include_directories("sjtu")
add_subdirectory(sjtd)
add_subdirectory(sjtt)
add_subdirectory(sjtu)
add_library(sjt $<TARGET_OBJECTS:sjtd> $<TARGET_OBJECTS:sjtt>
    $<TARGET_OBJECTS:sjtu>)
target_link_libraries(sjt
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/bde/build/groups/bdl/libbdl.a)
target_link_libraries(sjt
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/bde/build/groups/bsl/libbsl.a)
target_link_libraries(sjt
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/bde/build/thirdparty/decnumber/libdecnumber.a)
target_link_libraries(sjt
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/bde/build/thirdparty/inteldfp/libinteldfp.a)

