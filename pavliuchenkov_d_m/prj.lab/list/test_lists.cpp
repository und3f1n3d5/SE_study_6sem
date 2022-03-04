#include <iostream>

#include <cassert>
#include <string>
#include "list.h"

#include <ctime>

const int ELEMENTS_COUNT = 10;

struct TestStruct {
    std::string key;
    std::string value1;
    std::string value2;
};

typedef lab618::CSingleLinkedList<TestStruct> TestSingleList;
typedef lab618::CDualLinkedList<TestStruct> TestList;

static std::string makeRandomString(int minL = 7, int maxL = 14) {
    int length = rand() % maxL + minL;
    std::string s;
    s.reserve(length);
    // сделать случайную строку
    for (int i = 0; i < length - 3; i++) {
        char ch = rand() % 26 + 'a';
        s += ch;
    }
    s += "tmp";
    return s;
}

static void generate(TestStruct *pts) {
    pts->key = makeRandomString();
    pts->value1 = makeRandomString();
    pts->value2 = makeRandomString();
}



void TestClear() {
    TestSingleList single_list;
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
        TestStruct ts;
        generate(&ts);
        single_list.pushBack(ts);
    }

    assert(single_list.getSize() == ELEMENTS_COUNT);

    unsigned int start_time =  clock();
    single_list.clear();
    unsigned int stop_time =  clock();
    std::cout << (stop_time - start_time) / 1000.0;
}

int main() { TestClear(); }

// 0.002 vs 0.004
