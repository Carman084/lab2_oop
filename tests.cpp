#include "tests.h"
#include <chrono>

void SetTests::runTest(const std::string& testName, void (SetTests::*test)()) {
    auto start = std::chrono::steady_clock::now();
    try {
        (this->*test)();
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::steady_clock::now() - start).count();
        ++passed;
        std::cout << "[OK]   " << testName << " (" << ms << " us)\n";
    }
    catch (const std::exception& e) {
        ++failed;
        std::cout << "[FAIL] " << testName << ": " << e.what() << "\n";
    }
}

void SetTests::testPositive() {
    set<int> s;
    checkSet(s.getLength() == 0, "default ctor: length should be 0");

    s.add(1); s.add(2); s.add(3); s.add(2);
    checkSet(s.getLength() == 3, "add: duplicate should not be inserted");
    checkSet(s.contains(1) && s.contains(2) && s.contains(3), "contains: elements not found");
    checkSet(!s.contains(99), "contains: 99 should not be in set");

    s.remove(2);
    checkSet(s.getLength() == 2, "remove: length should decrease");
    checkSet(!s.contains(2), "remove: element still present");
    s.add(2);

    set<int> b({1, 2, 3, 2, 1});
    checkSet(b.getLength() == 3, "initializer_list: duplicates should be filtered");

    set<int> copy(s);
    checkSet(copy.getLength() == 3, "copy ctor: length mismatch");
    copy.add(4);
    checkSet(!s.contains(4), "copy ctor: original should not be affected");

    set<int> tmp(std::move(copy));
    checkSet(tmp.getLength() == 4, "move ctor: length mismatch");
    checkSet(copy.getLength() == 0, "move ctor: source should be empty");

    copy = s;
    checkSet(copy.getLength() == 3, "copy assign: length mismatch");
    s = s;
    checkSet(s.getLength() == 3, "self-assign: length should not change");

    tmp = std::move(copy);
    checkSet(tmp.getLength() == 3, "move assign: length mismatch");
    checkSet(copy.getLength() == 0, "move assign: source should be empty");

    int* arr = s.toArray();
    bool allFound = true;
    for (int i = 0; i < s.getLength() && allFound; ++i)
        if (!s.contains(arr[i]))
            allFound = false;
    delete[] arr;
    checkSet(allFound, "toArray: contains wrong elements");

    std::ostringstream oss;
    oss << s;
    checkSet(!oss.str().empty(), "operator<<: output should not be empty");

    tmp.clear();
    checkSet(tmp.getLength() == 0, "clear: length should be 0");
    checkSet(!tmp.contains(1), "clear: element should not be present");

    // s = {1, 2, 3} и b = {3, 4, 5}
    b = set<int>({3, 4, 5});
    set<int> u = s + b;
    checkSet(u.getLength() == 5, "operator+: union length should be 5");
    checkSet(u.contains(1) && u.contains(5), "operator+: missing elements");
    s += b;
    checkSet(s.getLength() == 5, "operator+=: union length should be 5");

    s = set<int>({1, 2, 3});
    b = set<int>({2, 3, 4});
    set<int> inter = s * b;
    checkSet(inter.getLength() == 2, "operator*: intersection length should be 2");
    checkSet(inter.contains(2) && inter.contains(3), "operator*: should contain 2 and 3");
    checkSet(!inter.contains(1) && !inter.contains(4), "operator*: should not contain 1 or 4");
    s *= b;
    checkSet(s.getLength() == 2, "operator*=: intersection length should be 2");

    s = set<int>({1, 2, 3});
    set<int> diff = s / b;
    checkSet(diff.getLength() == 1, "operator/: subtract length should be 1");
    checkSet(diff.contains(1), "operator/: should contain 1");
    checkSet(!diff.contains(2) && !diff.contains(3), "operator/: should not contain 2 or 3");
    s /= b;
    checkSet(s.getLength() == 1, "operator/=: subtract length should be 1");

    s = set<int>({10, 20, 30});
    int sum = 0;
    for (auto it = s.iteratorBegin(); !it.isEnd(); it.next())
        sum += it.value();
    checkIterator(sum == 60, "iterator (isEnd/next/value): sum mismatch");

    sum = 0;
    for (auto it = s.iteratorBegin(); it != s.iteratorEnd(); ++it)
        sum += *it;
    checkIterator(sum == 60, "iterator (++/*): sum mismatch");

    checkIterator(s.iteratorBegin() == s.iteratorBegin(), "iterator ==: begin should equal begin");
    checkIterator(s.iteratorBegin() != s.iteratorEnd(), "iterator !=: begin should not equal end");

    checkIterator(tmp.iteratorBegin().isEnd(), "iterator: empty set begin should be end");
}

void SetTests::testRemoveMissingElement() {
    set<int> s({1, 2, 3});
    bool caught = false;
    try {
        s.remove(67);
    }
    catch (const SetException&) {
        caught = true;
    }
    checkSet(caught, "should throw SetException on remove of missing element");
}

void SetTests::testIteratorValueAtEnd() {
    set<int> s({1, 2, 3});
    bool caught = false;
    try {
        s.iteratorEnd().value();
    }
    catch (const IteratorException&) {
        caught = true;
    }
    checkIterator(caught, "should throw IteratorException on value() at end");
}

void SetTests::testIteratorDerefAtEnd() {
    set<int> s({1, 2, 3});
    bool caught = false;
    try {
        *s.iteratorEnd();
    }
    catch (const IteratorException&) {
        caught = true;
    }
    checkIterator(caught, "should throw IteratorException on operator* at end");
}

void SetTests::testIteratorNextAtEnd() {
    set<int> s({1, 2, 3});
    bool caught = false;
    try {
        s.iteratorEnd().next();
    }
    catch (const IteratorException&) {
        caught = true;
    }
    checkIterator(caught, "should throw IteratorException on next() at end");
}

void SetTests::checkSet(bool value, const std::string& message) {
    if (!value)
        throw SetException(message);
}

void SetTests::checkIterator(bool value, const std::string& message) {
    if (!value)
        throw IteratorException(message);
}

int SetTests::runAllTests() {
    runTest("positive",                  &SetTests::testPositive);
    runTest("iterator value() at end",   &SetTests::testIteratorValueAtEnd);
    runTest("remove missing element",    &SetTests::testRemoveMissingElement);
    runTest("iterator operator* at end", &SetTests::testIteratorDerefAtEnd);
    runTest("iterator next() at end",    &SetTests::testIteratorNextAtEnd);

    std::cout << "\nPassed: " << passed
              << ", Failed: " << failed << "\n";
    return failed != 0;
}