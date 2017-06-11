add_library(sjtt OBJECT sjtt_bytecode.cpp sjtt_datumudtutil
    sjtt_executioncontext.cpp sjtt_frame.cpp)

add_executable(sjtt_bytecode.t sjtt_bytecode.t.cpp)
target_link_libraries(sjtt_bytecode.t sjt)
add_test(sjtt_bytecode sjtt_bytecode.t)

add_executable(sjtt_datumudtutil.t sjtt_datumudtutil.t.cpp)
target_link_libraries(sjtt_datumudtutil.t sjt)
add_test(sjtt_datumudtutil sjtt_datumudtutil.t)

add_executable(sjtt_executioncontext.t sjtt_executioncontext.t.cpp)
target_link_libraries(sjtt_executioncontext.t sjt)
add_test(sjtt_executioncontext sjtt_executioncontext.t)

add_executable(sjtt_frame.t sjtt_frame.t.cpp)
target_link_libraries(sjtt_frame.t sjt)
add_test(sjtt_frame sjtt_frame.t)
