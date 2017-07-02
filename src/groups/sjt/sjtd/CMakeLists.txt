add_library(sjtd OBJECT sjtd_datumudtutil sjtd_datumfactory)

# setup test drivers

add_executable(sjtd_datumudtutil.t sjtd_datumudtutil.t.cpp)
target_link_libraries(sjtd_datumudtutil.t sjt)
add_test(sjtd_datumudtutil sjtd_datumudtutil.t)

add_executable(sjtd_datumfactory.t sjtd_datumfactory.t.cpp)
target_link_libraries(sjtd_datumfactory.t sjt)
add_test(sjtd_datumfactory sjtd_datumfactory.t)
