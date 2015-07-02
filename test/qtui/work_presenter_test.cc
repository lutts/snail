//-*- TestCaseName: WorkPresenterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "pfmvp/mock_pf_triad_manager.h"

// triad headers
#include "snail/mock_work_model.h"
#include "qtui/mock_work_view.h"
#include "src/qtui/work_presenter.h"

// triad headers
#include "snail/mock_attribute_adder_model.h"
#include "qtui/mock_attribute_adder_dialog.h"

#include "qtui/mock_work_basic_info_qmodel.h"

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
    auto basicInfoQModel = utils::make_unique<MockWorkBasicInfoQModel>();

    RECORD_USED_MOCK_OBJECTS_SETUP;

    R_EXPECT_CALL(*view, setBasicInfoQModel(basicInfoQModel.get()));

    R_EXPECT_CALL(*view, whenUserClickAddAttribute(_, _))
        .WillOnce(SaveArg<0>(&userClickAddAttr));

    // Excercise system
    presenter = std::make_shared<WorkPresenter>(model, view,
                                                std::move(basicInfoQModel));
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
  SlotCatcher<IWorkView::UserClickAddAttributeSlotType> userClickAddAttr;
  // endregion
};

TEST_F(WorkPresenterTest, should_popup_add_attribute_dialog_when_UserClickAddAttribute) { // NOLINT
  // Setup fixture
  auto attr_adder_model = std::make_shared<MockAttributeAdderModel>();
  std::shared_ptr<IPfModel> attr_adder_pfmodel = attr_adder_model;
  auto attr_adder_dialog = std::make_shared<MockAttributeAdderDialog>();

  // Expectations
  EXPECT_CALL(*model, createAttributeAdderModel())
      .WillOnce(Return(attr_adder_model));
  EXPECT_CALL(triad_manager, createViewFor(attr_adder_pfmodel, _, _))
      .WillOnce(Return(attr_adder_dialog));
  EXPECT_CALL(*attr_adder_dialog, showView(true));

  // Exercise system
  userClickAddAttr();
}
