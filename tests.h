#ifndef TESTS_H
#define TESTS_H

#include<string>
#include "set.h"
#include "setexception.h"
#include "iteratorexception.h"
#include <iostream>
#include <sstream>
#include <utility>

class SetTests {
public:
    int runAllTests();
private:
    int passed = 0;
    int failed = 0;
    static void checkSet(bool value, const std::string& message);
    static void checkIterator(bool value, const std::string& message);
    void runTest(const std::string& testName, void (SetTests::*test)());
    void testPositive();
    void testRemoveMissingElement();
    void testIteratorValueAtEnd();
    void testIteratorDerefAtEnd();
    void testIteratorNextAtEnd();
};

#endif // TESTS_H
