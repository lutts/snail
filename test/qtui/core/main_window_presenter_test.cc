//-*- TestCaseName: MainWindowPresenterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "utils/u8string.h"
#include "test/testutils/utils.h"
#include "test/testutils/slot_catcher.h"
#include "pfmvp/mock_pf_triad_manager.h"

#include "snail/mock_main_window_model.h"
#include "qtui/ui/mock_main_window_view.h"
#include "src/qtui/core/main_window_presenter.h"

#include "snail/mock_workspace_model.h"
#include "qtui/ui/mock_workspace_view.h"

using utils::U8String;
using namespace snailcore;         // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;             // NOLINT
using namespace pfmvp::tests;      // NOLINT

class MainWindowPresenterTest : public ::testing::Test {
 protected:
  MainWindowPresenterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~MainWindowPresenterTest() { }
  virtual void SetUp() {
    model = std::make_shared<MockMainWindowModel>();
    view = std::make_shared<MockMainWindowView>();

    RECORD_USED_MOCK_OBJECTS_SETUP;

    // init window title
    utils::U8String dummyTitle = xtestutils::genRandomString();
    R_EXPECT_CALL(*model, windowTitle()).WillOnce(ReturnRef(dummyTitle));
    R_EXPECT_CALL(*view, setWindowTitle2(dummyTitle));

    // build central widget
    R_EXPECT_CALL(*model, getWorkSpaceModel())
        .WillOnce(Return(workspace_model));

    std::shared_ptr<IPfModel> workspace_pfmodel = workspace_model;
    R_EXPECT_CALL(triad_manager, createViewFor(workspace_pfmodel, _, _, _))
        .WillOnce(Return(workspace_view));

    R_EXPECT_CALL(*view, setWorkSpaceView(workspace_view.get()));

    // bind event handlers
    R_EXPECT_CALL(*model, whenWindowTitleChanged(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&windowTitleChanged), Return(SignalConnection())));

    R_EXPECT_CALL(*view, whenUserClickAddWork(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&userClickAddWork), Return(SignalConnection())));

    R_EXPECT_CALL(*view, whenRequestClose(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&userCloseWindow), Return(SignalConnection())));

    presenter = std::make_shared<MainWindowPresenter>(model, view);
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  std::shared_ptr<MockMainWindowModel> model;
  std::shared_ptr<MockMainWindowView> view;

  std::shared_ptr<MockWorkSpaceModel> workspace_model;
  std::shared_ptr<MockWorkSpaceView> workspace_view;

  MockPfTriadManager triad_manager;
  // endregion

  // region: test subject
  std::shared_ptr<MainWindowPresenter> presenter;
  // endregion

  // region: object depends on test subject
  using WindowTitleChangedSlotType =
      snailcore::IMainWindowModel::WindowTitleChangedSlotType;
  xtestutils::SlotCatcher<WindowTitleChangedSlotType> windowTitleChanged;

  xtestutils::SlotCatcher<IMainWindowView::UserClickAddWorkSlotType>
      userClickAddWork;
  xtestutils::SlotCatcher<IMainWindowView::RequestCloseSlotType>
      userCloseWindow;
  // endregion
};

TEST_F(MainWindowPresenterTest,
       should_update_window_title_when_model_title_changed) {  // NOLINT
  // expectations
  utils::U8String newTitle = xtestutils::genRandomString();
  EXPECT_CALL(*view, setWindowTitle2(newTitle));

  // Exercise system
  windowTitleChanged(newTitle);
}

TEST_F(MainWindowPresenterTest,
       should_request_close_when_use_close_window) {  // NOLINT
  // expectations
  auto tester = [this](bool expect_result) {
    EXPECT_CALL(*model, requestClose()).WillOnce(Return(expect_result));

    // Exercise system
    ASSERT_EQ(expect_result, userCloseWindow());
    Mock::VerifyAndClearExpectations(model.get());
  };

  tester(true);
  tester(false);
}

TEST_F(MainWindowPresenterTest,
       should_createWork_when_UserClickAddWork) {  // NOLINT
  // Setup fixture
  auto work_name = xtestutils::genRandomString();

  // Expectations
  EXPECT_CALL(*model, createWork(work_name));

  // Exercise system
  userClickAddWork(work_name);
}
