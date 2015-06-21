//-*- TestCaseName: WorkModelTest;-*-
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
#include "test/testutils/mock_object_generator.h"

#include "src/core/work_model.h"
#include "snail/mock_work.h"

namespace snailcore {
namespace tests {

class WorkModelTest : public ::testing::Test {
 protected:
  WorkModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkModelTest() { }
  virtual void SetUp() {
    EXPECT_CALL(work, whenBasicInfoChanged(_, _))
        .WillOnce(SaveArg<0>(&workBasicInfoChanged));

    auto model = std::make_shared<WorkModel>();
    model->set_work(&work);
    work_model = std::move(model);
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  MockWork work;
  // endregion

  // region: test subject
  std::shared_ptr<IWorkModel> work_model;
  // endregion

  // region: object depends on test subject
  SlotCatcher<IWork::BasicInfoChangedSlotType> workBasicInfoChanged;
  // endregion
};

class MockListener : public GenericMockListener<MockListener,
                                                IWorkModel> {
 public:
  MOCK_METHOD0(BasicInfoChanged, void());

  void bindListenerMethods(std::shared_ptr<utils::ITrackable> trackObject,
                           IWorkModel* model) {
    model->whenBasicInfoChanged(
        [this]() {
          BasicInfoChanged();
        },
        trackObject);
  }
};

TEST_F(WorkModelTest, should_name_be_the_backing_work_name) { // NOLINT
  // Setup fixture
  auto work_name = xtestutils::genRandomString();

  ON_CALL(work, name()).WillByDefault(ReturnRef(work_name));

  // Verify results
  ASSERT_EQ(work_name, work_model->name());
}

TEST_F(WorkModelTest, should_relay_set_name_to_backing_work) { // NOLINT
  // Setup fixture
  auto tester = [this](bool expect_result) {
    auto new_name = xtestutils::genRandomString();

    // Expectations
    EXPECT_CALL(work, set_name(new_name))
    .WillOnce(Return(expect_result));

    // Verify results
    ASSERT_EQ(expect_result, work_model->set_name(new_name));
  };

  tester(true);
  tester(false);
}

TEST_F(WorkModelTest, should_relay_BasicInfoChanged_signal_fired_by_backing_work) { // NOLINT
  // Expectations
  auto mockListener = MockListener::attachTo(work_model.get());
  EXPECT_CALL(*mockListener, BasicInfoChanged());

  // Exercise system
  workBasicInfoChanged();
}

}  // namespace tests
}  // namespace snailcore
