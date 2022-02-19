add_library(M3i M3i.h M3i.cpp)
set_property(TARGET M3i PROPERTY CXX_STANDARD 20)

add_executable(M3i_test M3i_test.cpp M3i_test.cpp)
target_link_libraries(M3i_test M3i)
