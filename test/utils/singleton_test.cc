//-*- TestCaseName: SingletonTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "src/utils/singleton.h"

using ::testing::GMOCK_FLAG(verbose);
using ::testing::internal::kErrorVerbosity;
using ::testing::internal::kInfoVerbosity;
using ::testing::internal::kWarningVerbosity;

namespace utils {

#define MAGIC_DATA 0xdeadbeaf

class TestClass : public Singleton<TestClass> {
 public:
  unsigned int magicData { MAGIC_DATA };
};

SINGLETON_STATIC_INSTANCE(TestClass)

namespace tests {

class SingletonTest : public ::testing::Test {
 protected:
  SingletonTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  ~SingletonTest() { }
  virtual void SetUp() { }
  virtual void TearDown() {
    TestClass::resetInstance();
  }
};

TEST_F(SingletonTest, hasInstanceInitiallyFalse) { // NOLINT
  // Verify results
  ASSERT_FALSE(TestClass::hasInstance());
}

TEST_F(SingletonTest, hasInstanceShouldReturnTrueAfterGetInstance) { // NOLINT
  TestClass::getInstance();

  // Verify results
  ASSERT_TRUE(TestClass::hasInstance());
}

TEST_F(SingletonTest, getInstanceShouldUsable) { // NOLINT
  // Verify results
  ASSERT_EQ(MAGIC_DATA, TestClass::getInstance().magicData);
}

TEST_F(SingletonTest, hasInstanceShouldReturnFalseAfterResetInstance) { // NOLINT
  // Exercise system
  TestClass::resetInstance();

  // Verify results
  ASSERT_FALSE(TestClass::hasInstance());
}

}  // namespace tests
}  // namespace utils
