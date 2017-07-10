add_library(sjtu OBJECT sjtu_bytecodedslutil.cpp sjtu_interpretutil.cpp)
add_library(sjtu_test sjtu_bytecodedslutil.cpp sjtu_interpretutil.cpp)
target_link_libraries(sjtu_test bdl bsl decnumber inteldfp sjtt_test sjtd_test)

add_executable(sjtu_bytecodedslutil.t sjtu_bytecodedslutil.t.cpp)
target_link_libraries(sjtu_bytecodedslutil.t sjtu_test)
add_test(sjtu_bytecodedslutil sjtu_bytecodedslutil.t)

add_executable(sjtu_interpretutil.t sjtu_interpretutil.t.cpp)
target_link_libraries(sjtu_interpretutil.t sjtu_test)
add_test(sjtu_interpretutil sjtu_interpretutil.t)
