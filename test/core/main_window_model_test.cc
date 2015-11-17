//-*- TestCaseName: MainWindowModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "utils/u8string.h"
#include "utils/i18n.h"
#include "test/testutils/utils.h"
#include "test/testutils/slot_catcher.h"
#include "test/testutils/generic_mock_listener.h"
#include "src/core/main_window_model.h"

#include "snail/mock_workspace_model.h"

namespace snailcore {
namespace tests {

class MainWindowModelTest : public ::testing::Test {
 protected:
  MainWindowModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~MainWindowModelTest() { }
  virtual void SetUp() {
    workspace_model = std::make_shared<MockWorkSpaceModel>();

    model = std::make_shared<MainWindowModel>(workspace_model);

    ASSERT_EQ(expect_init_title, model->windowTitle());
  }
  // virtual void TearDown() { }

  std::shared_ptr<MockWorkSpaceModel> workspace_model;

  std::shared_ptr<IMainWindowModel> model;
  utils::U8String expect_init_title{_("Snail")};
};

class MockListener
    : public GenericMockListener<MockListener, IMainWindowModel> {
 public:
  MOCK_METHOD1(WindowTitleChanged, void(const utils::U8String&));
  MOCK_METHOD0(RequestClose, bool());

  void bindListenerMethods(std::shared_ptr<utils::ITrackable> trackObject,
                           IMainWindowModel* model) {
    model->whenWindowTitleChanged([this](const utils::U8String& newTitle) {
      WindowTitleChanged(newTitle);
    }, trackObject);

    model->whenRequestClose([this]() -> bool { return RequestClose(); },
                            trackObject);
  }
};

TEST_F(MainWindowModelTest,
       should_get_new_title_after_setWindowTitle) {  // NOLINT
  // expectations
  utils::U8String expect_new_title =
      xtestutils::genRandomDifferentString(model->windowTitle());

  // Exercise system
  model->setWindowTitle(expect_new_title);

  // Verify results
  ASSERT_EQ(expect_new_title, model->windowTitle());
}

TEST_F(
    MainWindowModelTest,
    should_fire_WindowTitleChanged_event_when_set_different_title) {  // NOLINT
  // expectations
  utils::U8String expect_new_title =
      xtestutils::genRandomDifferentString(model->windowTitle());
  auto mockListener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mockListener, WindowTitleChanged(expect_new_title));

  // Exercise system
  model->setWindowTitle(expect_new_title);
}

TEST_F(MainWindowModelTest,
       should_not_fire_WindowTitleChanged_when_set_same_title) {  // NOLINT
  // expectations
  // should not call any method on strict mock
  auto mockListener = MockListener::attachStrictTo(model.get());

  // Exercise system
  model->setWindowTitle(model->windowTitle());
}

TEST_F(MainWindowModelTest,
       should_requestClose_default_return_true_when_no_listeners) {  // NOLINT
  // Verify results
  ASSERT_TRUE(model->requestClose());
}

TEST_F(MainWindowModelTest,
       should_fire_RequestClose_when_requestClose_called) {  // NOLINT
  auto tester = [this](bool expect_result) {
    // expectations
    auto mockListener = MockListener::attachTo(model.get());
    EXPECT_CALL(*mockListener, RequestClose()).WillOnce(Return(expect_result));

    // Exercise system
    ASSERT_EQ(expect_result, model->requestClose());
  };

  tester(true);
  tester(false);
}

TEST_F(
    MainWindowModelTest,
    should_and_all_listener_response_when_fire_RequestClose_event) {  // NOLINT
  auto tester = [this](bool response1, bool response2, bool expect_result) {
    // expectations
    auto mockListener1 = MockListener::attachTo(model.get());
    EXPECT_CALL(*mockListener1, RequestClose()).WillOnce(Return(response1));

    auto mockListener2 = MockListener::attachTo(model.get());
    EXPECT_CALL(*mockListener2, RequestClose())
        .Times(AtMost(1))
        .WillOnce(Return(response2));

    // Exercise system
    auto actual_result = model->requestClose();

    // Verify results
    ASSERT_EQ(expect_result, actual_result);
  };

  // Four possible combinations
  tester(true, true, true & true);
  tester(true, false, true & false);
  tester(false, true, false & true);
  tester(false, false, false & false);
}

TEST_F(MainWindowModelTest, should_be_able_to_get_workspace_model) {  // NOLINT
  ASSERT_EQ(workspace_model, model->getWorkSpaceModel());
}

TEST_F(MainWindowModelTest,
       DISABLED_should_relay_createWork_to_workspace_model) {  // NOLINT
  // Setup fixture
  auto work_name = xtestutils::genRandomString();

  // Expectations
  EXPECT_CALL(*workspace_model, createWork(work_name));

  // Exercise system
  model->createWork(work_name);
}

}  // namespace tests
}  // namespace snailcore
