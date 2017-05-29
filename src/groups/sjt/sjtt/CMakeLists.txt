add_library(sjtt OBJECT sjtt_bytecode.cpp sjtt_executioncontext.cpp)

add_executable(sjtt_bytecode.t sjtt_bytecode.t.cpp)
target_link_libraries(sjtt_bytecode.t sjt)
add_test(sjtt_bytecode sjtt_bytecode.t)

add_executable(sjtt_executioncontext.t sjtt_executioncontext.t.cpp)
target_link_libraries(sjtt_executioncontext.t sjt)
add_test(sjtt_executioncontext sjtt_executioncontext.t)
