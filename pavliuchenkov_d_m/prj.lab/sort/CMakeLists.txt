add_library(sort sort.h sort.cpp)
set_property(TARGET sort PROPERTY CXX_STANDARD 20)

add_executable(test_sort test_sort.cpp)
target_link_libraries(test_sort sort)
