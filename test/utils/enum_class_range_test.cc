//-*- TestCaseName: EnumClassRangeTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/utils/utils.h"

using ::testing::GMOCK_FLAG(verbose);
using ::testing::internal::kErrorVerbosity;
using ::testing::internal::kInfoVerbosity;
using ::testing::internal::kWarningVerbosity;

namespace utils {
namespace tests {

class EnumClassRangeTest : public ::testing::Test {
 protected:
  EnumClassRangeTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  ~EnumClassRangeTest() { }
  virtual void SetUp() { }
  virtual void TearDown() { }
};

enum class test_enum {
  zero,
    one,
    two,
    three,
    count
};

TEST_F(EnumClassRangeTest, iterateOverEnumClassRangeShouldGotAllPossibleValues) { // NOLINT
  // expectations
  test_enum expect_values[] = {
    test_enum::zero,
    test_enum::one,
    test_enum::two,
    test_enum::three
  };

  // Verify results
  int i = 0;
  for (const auto& e : enum_class_range<test_enum>()) {
    ASSERT_EQ(expect_values[i], e);
    ++i;
  }
}

TEST_F(EnumClassRangeTest, enumWithSpecifiedFirst) { // NOLINT
  // Setup fixture

  // expectations
  test_enum expect_values[] = {
    test_enum::two,
    test_enum::three
  };

  // Verify results
  int i = 0;
  for (const auto& e : enum_class_range<test_enum, test_enum::two>()) {
    ASSERT_EQ(expect_values[i], e);
    ++i;
  }
}

TEST_F(EnumClassRangeTest, enumWithSpecifiedFirstLast) { // NOLINT
  // expectations
  test_enum expect_values[] = {
    test_enum::one,
    test_enum::two
  };

  // Verify results
  int i = 0;
  for (const auto& e : enum_class_range<test_enum,
           test_enum::one, test_enum::three>()) {
    ASSERT_EQ(expect_values[i], e);
    ++i;
  }
}

}  // namespace tests
}  // namespace utils
