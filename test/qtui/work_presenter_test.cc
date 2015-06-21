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
#include "test/testutils/model_view_mock_generator.h"

// triad headers
#include "snail/mock_work_model.h"
#include "qtui/mock_work_view.h"
#include "src/qtui/work_presenter.h"

#include "qtui/mock_work_basic_info_qmodel.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

class WorkPresenterTest : public ::testing::Test {
  //-*- TestCaseName: WorkPresenterTest;-*-
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
    auto basicInfoQModel = utils::make_unique<MockWorkBasicInfoQModel>();

    RECORD_USED_MOCK_OBJECTS_SETUP;

    R_EXPECT_CALL(*view, setBasicInfoQModel(basicInfoQModel.get()));

    // Excercise system
    presenter = std::make_shared<WorkPresenter>(model, view,
                                                std::move(basicInfoQModel));
    presenter->initialize();
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  std::shared_ptr<MockWorkModel> model;
  std::shared_ptr<MockWorkView> view;
  // endregion

  // region: test subject
  std::shared_ptr<WorkPresenter> presenter;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(WorkPresenterTest, should_construct_properly) { // NOLINT
  // See SetUp()
  SUCCEED();
}
