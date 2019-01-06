//
// Created by Saxion ACS.
//

#ifndef LISTS_TEST_RUNNER_H
#define LISTS_TEST_RUNNER_H

#include "gtest/gtest.h"
#include <iostream>

class test_runner : public ::testing::EmptyTestEventListener {

public:
    test_runner(std::ostream& out) :
            EmptyTestEventListener(),
            _out(out) {}

    test_runner() :
            test_runner(std::cout) {

    }

private:

    std::ostream& _out;

public:
    // called before any test activity starts.

    virtual void OnTestProgramStart([[maybe_unused]] const ::testing::UnitTest& test) override {
    }

    // called after all test activities have ended.
    virtual void OnTestProgramEnd([[maybe_unused]] const ::testing::UnitTest& unit_test) override {
    }

    // this is called before each run iteration
    void OnTestIterationStart([[maybe_unused]]  const ::testing::UnitTest& test, [[maybe_unused]] int i) override {
    }

    // and this after
    void OnTestIterationEnd([[maybe_unused]] const ::testing::UnitTest& test, [[maybe_unused]] int i) override {
    }

    // called before each suite
    virtual void OnTestCaseStart([[maybe_unused]] const ::testing::TestCase& test_case) override {
    }

    // called after each suite
    virtual void OnTestCaseEnd([[maybe_unused]] const ::testing::TestCase& test_case) override {
    }

    // called on each test case start
    virtual void OnTestStart([[maybe_unused]] const ::testing::TestInfo& test_info) override {
    }

    // called after a failed assertion or a SUCCEED() invocation.
    virtual void OnTestPartResult([[maybe_unused]] const ::testing::TestPartResult& test_part_result) override {
    }

    // called on each test case end
    virtual void OnTestEnd([[maybe_unused]] const ::testing::TestInfo& test_info) override {
    }

};


#endif //LISTS_TEST_RUNNER_H
