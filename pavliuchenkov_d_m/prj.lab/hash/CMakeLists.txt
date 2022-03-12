add_library(hash hash.h hash.cpp)
set_property(TARGET hash PROPERTY CXX_STANDARD 20)

add_executable(hash_test hash_test.cpp)
target_link_libraries(hash_test hash)
