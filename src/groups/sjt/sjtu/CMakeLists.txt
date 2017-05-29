add_library(sjtu OBJECT sjtu_datumutil.cpp sjtu_interpretutil.cpp)

add_executable(sjtu_datumutil.t sjtu_datumutil.t.cpp)
target_link_libraries(sjtu_datumutil.t sjt)
add_test(sjtu_datumutil sjtu_datumutil.t)

add_executable(sjtu_interpretutil.t sjtu_interpretutil.t.cpp)
target_link_libraries(sjtu_interpretutil.t sjt)
add_test(sjtu_interpretutil sjtu_interpretutil.t)
