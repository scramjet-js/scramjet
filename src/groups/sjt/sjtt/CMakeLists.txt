add_library(sjtt OBJECT sjtt_bytecode.cpp
    sjtt_executioncontext.cpp sjtt_frame.cpp sjtt_function)
add_library(sjtt_test sjtt_bytecode.cpp
    sjtt_executioncontext.cpp sjtt_frame.cpp)
target_link_libraries(sjtt_test bdl bsl decnumber inteldfp sjtd_test)

add_executable(sjtt_bytecode.t sjtt_bytecode.t.cpp)
target_link_libraries(sjtt_bytecode.t sjtt_test)
add_test(sjtt_bytecode sjtt_bytecode.t)

add_executable(sjtt_executioncontext.t sjtt_executioncontext.t.cpp)
target_link_libraries(sjtt_executioncontext.t sjtt_test)
add_test(sjtt_executioncontext sjtt_executioncontext.t)

add_executable(sjtt_frame.t sjtt_frame.t.cpp)
target_link_libraries(sjtt_frame.t sjtt_test)
add_test(sjtt_frame sjtt_frame.t)

add_executable(sjtt_function.t sjtt_function.t.cpp)
target_link_libraries(sjtt_function.t sjt)
add_test(sjtt_function sjtt_function.t)
