//-*- TestCaseName: WorkPresenterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "pfmvp/mock_pf_triad_manager.h"

#include "test/testutils/utils.h"
#include "test/testutils/random_string.h"

// triad headers
#include "snail/mock_work_model.h"
#include "qtui/mock_work_view.h"
#include "src/qtui/core/work_presenter.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;  // NOLINT
using namespace pfmvp::tests;  // NOLINT

class WorkPresenterTest : public ::testing::Test {
 protected:
  WorkPresenterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkPresenterTest() { }
  virtual void SetUp() {
    // Setup fixture
    model = std::make_shared<MockWorkModel>();
    view = std::make_shared<MockWorkView>();

    RECORD_USED_MOCK_OBJECTS_SETUP;

    xtestutils::RandomString work_name;
    R_EXPECT_CALL(*model, name()).WillOnce(Return(work_name.ustr()));
    R_EXPECT_CALL(*view, setWorkName(work_name.qstr()));

#if 0
    R_EXPECT_CALL(*model, description()).WillOnce(Return(work_desc));
    R_EXPECT_CALL(*view, setWorkDescription());
#endif

    R_EXPECT_CALL(*view, whenUserSetWorkName(_, _))
        .WillOnce(SaveArg<0>(&userSetWorkName));

    // Excercise system
    presenter = std::make_shared<WorkPresenter>(model, view);
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  std::shared_ptr<MockWorkModel> model;
  std::shared_ptr<MockWorkView> view;

  MockPfTriadManager triad_manager;
  // endregion

  // region: test subject
  std::shared_ptr<WorkPresenter> presenter;
  // endregion

  // region: object depends on test subject
  using UserSetWorkNameSlotType =
      IWorkView::UserSetWorkNameSlotType;
  SlotCatcher<UserSetWorkNameSlotType> userSetWorkName;
  // endregion
};

TEST_F(WorkPresenterTest,
       should_set_new_name_to_model_when_user_set_work_name) { // NOLINT
  // Setup fixture
  xtestutils::RandomString new_name;

  // Expectations
  EXPECT_CALL(*model, set_name(new_name.ustr()));

  // Exercise system
  userSetWorkName(new_name.qstr());
}
