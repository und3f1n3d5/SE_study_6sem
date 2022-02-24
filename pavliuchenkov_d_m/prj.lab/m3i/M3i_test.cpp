#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "M3i.h"

// enlarge
// 0_1 vs 0_2
TEST_CASE("Construct & copy") {
    M3i test;
    int64_t ptr[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    CHECK_NOTHROW(M3i a(2, 2, 2, ptr));
    M3i a(2, 2, 2, ptr);
    CHECK_NOTHROW(test = a);
}

TEST_CASE("Get/Set") {
    int64_t ptr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    M3i test(2, 2, 2, ptr);
    CHECK_NOTHROW(test.SetElement(10, 1, 1, 1));
    CHECK(test.GetElement(1, 1, 1) == 10);
    CHECK(test.GetElement(0, 0, 0) == 0);
    CHECK_THROWS(test.SetElement(1, 10, 1, 0));
    CHECK_THROWS(test.GetElement(1, 1, 10));
}

TEST_CASE("Resize") {
    int64_t ptr[8] = {8, 1, 2, 3, 4, 5, 6, 7};
    M3i test(2, 2, 2, ptr);
    CHECK_NOTHROW(test.Resize(1,1,1));
    CHECK(test.GetSize() == 1);
    CHECK_NOTHROW(test.Resize(2, 2, 2));
    CHECK(test.GetElement(1, 1, 1) == 0);
    CHECK(test.GetElement(0, 0, 0) == 8);
}

TEST_CASE("Copy & clone") {
    int64_t ptr1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int64_t ptr2[8] = {0, 1, 2, 3, 4, 5, 6, 9};
    M3i test1(2, 2, 2, ptr1);
    {
        M3i test_cp = test1.copy();
        M3i test_cn = test_cp.clone();
        test_cp.SetElement(2, 0, 0, 0);
        test_cn.SetElement(3, 0, 0, 0);
        CHECK(test1.GetElement(0, 0, 0) == 2);
    }
    CHECK(test1.GetElement(0, 0, 0) == 2);
}
