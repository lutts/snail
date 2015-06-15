//-*- TestCaseName: RangeTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <map>

#include "utils/utils.h"

using ::testing::GMOCK_FLAG(verbose);
using ::testing::internal::kErrorVerbosity;
using ::testing::internal::kInfoVerbosity;
using ::testing::internal::kWarningVerbosity;

namespace utils {
namespace tests {

class RangeTest : public ::testing::Test {
 protected:
  RangeTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  ~RangeTest() { }
  virtual void SetUp() { }
  virtual void TearDown() { }
};

TEST_F(RangeTest, testNonConstRange) { // NOLINT
  // Setup fixture
  std::vector<int> test_vec { 1, 2, 3, 4 };

  // Verify results
  int i = 0;
  for (int item : make_range(test_vec.begin(), test_vec.end())) {
    ASSERT_EQ(test_vec[i], item);
    ++i;
  }
}

TEST_F(RangeTest, testConstRange) { // NOLINT
  // Setup fixture
  struct TestItem {
    int dummy;

    bool operator==(const TestItem& other) const {
      return dummy == other.dummy;
    }
  };
  std::vector<TestItem> test_vec {
    TestItem{1}, TestItem{2}, TestItem{3}, TestItem{4} };

  // Verify results
  int i = 0;
  for (TestItem const& item : make_range(test_vec.begin(), test_vec.end())) {
    ASSERT_EQ(test_vec[i], item);
    ++i;
  }
}

TEST_F(RangeTest, rangeSizeIsDistanceOfIterator) { // NOLINT
  // Setup fixture
  std::vector<int> test_vec { 1, 2, 3, 4 };

  // Exercise system
  auto rng = make_range(test_vec.begin(), test_vec.end());

  // Verify results
  ASSERT_EQ(std::distance(test_vec.begin(), test_vec.end()),
            rng.distance());
}

TEST_F(RangeTest, testEqualRange) { // NOLINT
  // Setup fixture
  std::multimap<char, int> mymm;

  mymm.insert(std::pair<char, int>('a', 10));

  mymm.insert(std::pair<char, int>('b', 20));
  mymm.insert(std::pair<char, int>('b', 30));
  mymm.insert(std::pair<char, int>('b', 40));

  mymm.insert(std::pair<char, int>('c', 50));
  mymm.insert(std::pair<char, int>('c', 60));

  mymm.insert(std::pair<char, int>('d', 60));

  // Verify results
  for (char ch = 'a'; ch <= 'd'; ++ch) {
    // build expectations
    std::pair <std::multimap<char, int>::iterator,
               std::multimap<char, int>::iterator> expect_range;
    expect_range = mymm.equal_range(ch);
    std::multimap<char, int>::iterator expect_start = expect_range.first;
    std::multimap<char, int>::iterator expect_end = expect_range.second;

    // Verify results
    for (auto item : make_range(mymm.equal_range(ch))) {
      ASSERT_NE(expect_start, expect_end);
      ASSERT_EQ(*expect_start, item);
      ++expect_start;
    }

    ASSERT_EQ(expect_start, expect_end);
  }
}

}  // namespace tests
}  // namespace utils
