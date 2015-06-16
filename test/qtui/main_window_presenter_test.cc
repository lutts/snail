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

#include "snail/mock_main_window_model.h"
#include "qtui/mock_main_window_view.h"
#include "src/qtui/main_window_presenter.h"

using utils::U8String;
using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

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

    utils::U8String dummyTitle = xtestutils::genRandomString();
    R_EXPECT_CALL(*model, windowTitle())
        .WillOnce(ReturnRef(dummyTitle));
    R_EXPECT_CALL(*view, setWindowTitle2(dummyTitle));

    R_EXPECT_CALL(*model, whenWindowTitleChanged(_, _))
        .WillOnce(SaveArg<0>(&windowTitleChanged));

    R_EXPECT_CALL(*view, whenRequestClose(_, _))
        .WillOnce(SaveArg<0>(&userCloseWindow));

    presenter = MainWindowPresenter::create(model, view);
    presenter->initialize();
  }
  // virtual void TearDown() { }

  std::shared_ptr<MockMainWindowModel> model;
  std::shared_ptr<MockMainWindowView> view;

  std::shared_ptr<MainWindowPresenter> presenter;

  using WindowTitleChangedSlotType =
      snailcore::IMainWindowModel::WindowTitleChangedSlotType;
  SlotCatcher<WindowTitleChangedSlotType> windowTitleChanged;

  SlotCatcher<IMainWindowView::RequestCloseSlotType> userCloseWindow;
};

TEST_F(MainWindowPresenterTest, should_update_window_title_when_model_title_changed) { // NOLINT
  // expectations
  utils::U8String newTitle = xtestutils::genRandomString();
  EXPECT_CALL(*view, setWindowTitle2(newTitle));

  // Exercise system
  windowTitleChanged(newTitle);
}

TEST_F(MainWindowPresenterTest, should_request_close_when_use_close_window) { // NOLINT
  // expectations
  auto tester = [this](bool expect_result) {
    EXPECT_CALL(*model, requestClose())
    .WillOnce(Return(expect_result));

    // Exercise system
    ASSERT_EQ(expect_result, userCloseWindow());
    Mock::VerifyAndClearExpectations(model.get());
  };

  tester(true);
  tester(false);
}
