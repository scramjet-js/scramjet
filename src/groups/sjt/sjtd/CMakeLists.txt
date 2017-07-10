add_library(sjtd OBJECT sjtd_datumudtutil.cpp sjtd_datumfactory.cpp)
add_library(sjtd_test sjtd_datumudtutil.cpp sjtd_datumfactory.cpp)
target_link_libraries(sjtd_test bdl bsl decnumber inteldfp)

# setup test drivers

add_executable(sjtd_datumudtutil.t sjtd_datumudtutil.t.cpp)
target_link_libraries(sjtd_datumudtutil.t sjtd_test)
add_test(sjtd_datumudtutil sjtd_datumudtutil.t)

add_executable(sjtd_datumfactory.t sjtd_datumfactory.t.cpp)
target_link_libraries(sjtd_datumfactory.t sjtd_test)
add_test(sjtd_datumfactory sjtd_datumfactory.t)
