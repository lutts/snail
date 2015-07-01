//-*- TestCaseName: CandidateItemAnyDataTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <memory>
#include <string>

#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/utils.h"

#include "src/core/candidate_item_any_data.h"

namespace snailcore {
namespace tests {

class CandidateItemAnyDataTest : public ::testing::Test {
 protected:
  CandidateItemAnyDataTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~CandidateItemAnyDataTest() { }
  virtual void SetUp() {
  }
  // virtual void TearDown() { }
};

class CustomData {
 public:
  CustomData(int a, int b, int c)
      : a_(a), b_(b), c_(c) { }
  ~CustomData() = default;

  bool operator==(const CustomData& rhs) const {
    return (a_ == rhs.a_) &&
        (b_ == rhs.b_) &&
        (c_ == rhs.c_);
  }

 private:
  int a_;
  int b_;
  int c_;
};

TEST_F(CandidateItemAnyDataTest,
       should_be_able_to_set_and_get_back_any_data) { // NOLINT
  CandidateItemAnyData item;

  // string
  auto expect_str = xtestutils::genRandomString();
  item.set_data(expect_str);

  // Verify results
  auto actual_str = item.data<utils::U8String>();
  ASSERT_EQ(expect_str, actual_str);


  // shared_ptr
  auto expect_sp = std::make_shared<CustomData>(std::rand(),
                                                std::rand(),
                                                std::rand());
  item.set_data(expect_sp);
  // verify results
  auto actual_sp = item.data<std::shared_ptr<CustomData> >();
  ASSERT_EQ(expect_sp, actual_sp);
}

TEST_F(CandidateItemAnyDataTest,
       test_CandidateItemData_with_str) { // NOLINT
  // string
  CandidateItemData<utils::U8String> item;

  auto expect_str = xtestutils::genRandomString();
  item.set_data(expect_str);

  // Verify results
  auto actual_str = item.data();
  ASSERT_EQ(expect_str, actual_str);
}
TEST_F(CandidateItemAnyDataTest,
       test_CandiateItemData_with_shared_ptr) { // NOLINT
  CandidateItemData<std::shared_ptr<CustomData> > item;
  auto expect_sp = std::make_shared<CustomData>(std::rand(),
                                                std::rand(),
                                                std::rand());
  item.set_data(expect_sp);
  // verify results
  auto actual_sp = item.data();
  ASSERT_EQ(expect_sp, actual_sp);
}

}  // namespace tests
}  // namespace snailcore
