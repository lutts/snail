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
#include "qtui/ui/mock_work_view.h"
#include "src/qtui/core/work_presenter.h"

#include "snail/mock_attribute_set_model.h"
#include "qtui/ui/mock_attribute_set_view.h"

using namespace snailcore;         // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;             // NOLINT
using namespace pfmvp::tests;      // NOLINT

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

    // create attribute set view
    createAttributeSetView(&mock_obj_recorder);

    R_EXPECT_CALL(*view, whenUserSetWorkName(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&userSetWorkName), Return(SignalConnection())));

    // Excercise system
    presenter = std::make_shared<WorkPresenter>(model, view);
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;

    ASSERT_EQ(PF_VIEW_FACTORY(AttributeSetViewForWorkViewFactory),
              create_attr_set_view_args.view_factory_id());
  }
  // virtual void TearDown() { }

  void createAttributeSetView(xtestutils::MockObjectRecorder* mock_recorder);

  // region: objects test subject depends on
  std::shared_ptr<MockWorkModel> model;
  std::shared_ptr<MockWorkView> view;

  MockPfTriadManager triad_manager;
  PfCreateViewArgs create_attr_set_view_args;
  // endregion

  // region: test subject
  std::shared_ptr<WorkPresenter> presenter;
  // endregion

  // region: object depends on test subject
  using UserSetWorkNameSlotType = IWorkView::UserSetWorkNameSlotType;
  xtestutils::SlotCatcher<UserSetWorkNameSlotType> userSetWorkName;
  // endregion
};

void WorkPresenterTest::createAttributeSetView(
    xtestutils::MockObjectRecorder* mock_recorder) {
  xtestutils::MockObjectRecorder& mock_obj_recorder = *mock_recorder;

  auto attr_set_model = std::make_shared<MockAttributeSetModel>();
  auto attr_set_view = std::make_shared<MockAttributeSetView>();

  R_EXPECT_CALL(*model, createAttributeSetModel())
      .WillOnce(Return(attr_set_model));
  std::shared_ptr<IPfModel> attr_set_pmodel = attr_set_model;
  R_EXPECT_CALL(triad_manager, createViewFor(attr_set_pmodel, _, _, _))
      .WillOnce(DoAll(SaveArgPointee<MockPfTriadManager::kCreateViewArgsIdx>(
                          &create_attr_set_view_args),
                      Return(attr_set_view)));
  R_EXPECT_CALL(*view, setWorkAttrSetView(attr_set_view.get()));
}

TEST_F(WorkPresenterTest,
       should_set_new_name_to_model_when_user_set_work_name) {  // NOLINT
  // Setup fixture
  xtestutils::RandomString new_name;

  // Expectations
  EXPECT_CALL(*model, set_name(new_name.ustr()));

  // Exercise system
  userSetWorkName(new_name.qstr());
}
