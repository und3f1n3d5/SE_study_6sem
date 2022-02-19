add_library(list list.h list.cpp)
set_property(TARGET list PROPERTY CXX_STANDARD 20)

add_executable(list_test test_lists.cpp)
target_link_libraries(list_test list)
