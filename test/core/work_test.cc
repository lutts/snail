//-*- TestCaseName: WorkTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/utils.h"
#include "test/testutils/generic_mock_listener.h"
#include "test/testutils/slot_catcher.h"

#include "src/core/work.h"

namespace snailcore {
namespace tests {

class WorkTest : public ::testing::Test {
 protected:
  WorkTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkTest() { }
  virtual void SetUp() {
    work = utils::make_unique<Work>();

    ASSERT_EQ(utils::U8String{""}, work->name());
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  // endregion

  // region: test subject
  std::unique_ptr<IWork> work;
  // endregion

  // region: object depends on test subject
  // endregion
};

class MockListener : public GenericMockListener<MockListener,
                                                IWork> {
 public:
  MOCK_METHOD0(BasicInfoChanged, void());

  void bindListenerMethods(std::shared_ptr<utils::ITrackable> trackObject,
                           IWork* work) {
    work->whenBasicInfoChanged(
        [this]() {
          BasicInfoChanged();
        },
        trackObject);
  }
};

TEST_F(WorkTest, should_be_able_to_set_and_get_name) { // NOLINT
  // Setup fixture
  auto work_name = xtestutils::genRandomString();

  // Expectations

  // Exercise system
  work->set_name(work_name);

  // Verify results
  ASSERT_EQ(work_name, work->name());
}

TEST_F(WorkTest, should_fire_BasicInfoChanged_when_set_a_different_name) { // NOLINT
  // Setup fixture
  auto new_name = xtestutils::genRandomDifferentString(work->name());

  // Expectations
  auto mockListener = MockListener::attachTo(work.get());
  EXPECT_CALL(*mockListener, BasicInfoChanged());

  // Exercise system
  ASSERT_TRUE(work->set_name(new_name));
}

TEST_F(WorkTest, should_not_fire_BasicInfoChnaged_when_set_a_same_name) { // NOLINT
  // Expectations
  auto mockListener = MockListener::attachTo(work.get());
  EXPECT_CALL(*mockListener, BasicInfoChanged()).Times(0);

  // Exercise system
  ASSERT_FALSE(work->set_name(work->name()));
}

}  // namespace tests
}  // namespace snailcore
