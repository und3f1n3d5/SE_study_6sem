#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "m3i.h"

// enlarge
// 0_1 vs 0_2
TEST_CASE("Construct & copy") {
    CHECK_NOTHROW(M3i a(2, 2, 2));
    M3i test, b(2, 2, 2);
    int64_t ptr[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    CHECK_NOTHROW(test = b);
}

TEST_CASE("Get/Set") {
    int64_t ptr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    M3i test(2, 2, 2);
    CHECK_NOTHROW(test.SetElement(10, 1, 1, 1));
    CHECK(test.at(1, 1, 1) == 10);
    CHECK_THROWS(test.SetElement(1, 10, 1, 0));
    CHECK_THROWS(test.at(1, 1, 10));
}

TEST_CASE("Resize") {
    int64_t ptr[8] = {8, 1, 2, 3, 4, 5, 6, 7};
    M3i test(2, 2, 2);
    test.SetElement(8, 0, 0, 0);
    CHECK_NOTHROW(test.Resize(1,1,1));
    CHECK(test.GetSize() == 1);
    CHECK_NOTHROW(test.Resize(2, 2, 2));
    CHECK(test.at(1, 1, 1) == 0);
    CHECK(test.at(0, 0, 0) == 8);
}

TEST_CASE("Copy & clone") {
    int64_t ptr1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int64_t ptr2[8] = {0, 1, 2, 3, 4, 5, 6, 9};
    M3i test1(2, 2, 2);
    {
        M3i test_cp = test1.copy();
        M3i test_cn = test_cp.clone();
        test_cp.SetElement(2, 0, 0, 0);
        test_cn.SetElement(3, 0, 0, 0);
        CHECK(test1.at(0, 0, 0) == 2);
        test_cp.Resize(1, 1, 1);
        CHECK_NOTHROW(test_cn.at(1, 1, 1));
    }
    CHECK(test1.at(0, 0, 0) == 2);
    CHECK_THROWS(test1.at(1, 1, 1));
}

TEST_CASE("input and output") {
    int size[3] = {3, 2, 3};

    M3i m1(size[0], size[1], size[2]);

    // Filling tensor with random values
    for (int x_id = 0; x_id < size[0]; ++x_id) {
        for (int y_id = 0; y_id < size[1]; ++y_id) {
            for (int z_id = 0; z_id < size[2]; ++z_id) {
                m1.at(x_id, y_id, z_id) = rand();
            }
        }
    }

    std::stringstream str;
    M3i m2;

    str << m1;

    str >> m2;

    for (int x_id = 0; x_id < size[0]; ++x_id) {
        for (int y_id = 0; y_id < size[1]; ++y_id) {
            for (int z_id = 0; z_id < size[2]; ++z_id) {
                CHECK(m1.at(x_id, y_id, z_id) == m2.at(x_id, y_id, z_id));
            }
        }
    }
}
