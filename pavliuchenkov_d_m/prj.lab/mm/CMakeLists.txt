add_library(mm mm.h mm.cpp mm.cpp)
set_property(TARGET mm PROPERTY CXX_STANDARD 20)

add_executable(mm_test test_mm.cpp)
target_link_libraries(mm_test list)
