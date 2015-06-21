//-*- TestCaseName: WorkSpacePresenterTest;-*-
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
#include "pfmvp/mock_pf_triad_manager.h"

// triad headers
#include "snail/mock_workspace_model.h"
#include "qtui/mock_workspace_view.h"
#include "src/qtui/workspace_presenter.h"

#include "snail/mock_work_model.h"
#include "qtui/mock_work_view.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;  // NOLINT
using namespace pfmvp::tests;  // NOLINT

using AboutToDestroyModelSlot =
    SlotCatcher<IPfTriadManager::AboutToDestroyModelSlotType>;
using AboutToDestroyViewSlot =
    SlotCatcher<IPfTriadManager::AboutToDestroyViewSlotType>;

class WorkSpacePresenterTest : public ::testing::Test {
 protected:
  WorkSpacePresenterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkSpacePresenterTest() { }
  virtual void SetUp() {
    model = std::make_shared<MockWorkSpaceModel>();
    view = std::make_shared<MockWorkSpaceView>();

    RECORD_USED_MOCK_OBJECTS_SETUP;

    R_EXPECT_CALL(*view, whenUserClickAddWork(_, _))
        .WillOnce(SaveArg<0>(&userClickAddWork));

    R_EXPECT_CALL(*view, whenUserCloseWork(_, _))
        .WillOnce(SaveArg<0>(&userCloseWork));

    R_EXPECT_CALL(*model, whenWorkModelAdded(_, _))
        .WillOnce(SaveArg<0>(&workModelAdded));

    R_EXPECT_CALL(*model, whenActiveWorkModelChanged(_, _))
        .WillOnce(SaveArg<0>(&activeWorkModelChanged));

    R_EXPECT_CALL(*model, whenWorkModelActivelyRemoved(_, _))
        .WillOnce(SaveArg<0>(&workModelActivelyRemoved));

    presenter = std::make_shared<WorkSpacePresenter>(model, view);
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();
  }
  // virtual void TearDown() { }

  using WorkBasicInfoChangedSlot = IWorkModel::BasicInfoChangedSlotType;
  struct CreateWorkTriadResult {
    MockWorkModel* model;
    MockWorkView* view;
    std::unique_ptr<AboutToDestroyModelSlot> aboutToDestroyModelSlot;
    std::unique_ptr<AboutToDestroyViewSlot> aboutToDestroyViewSlot;
    std::unique_ptr<WorkBasicInfoChangedSlot> workBasicInfoChanged;
  };
  void createWorkTriad(CreateWorkTriadResult* result = nullptr);

  // region: objects test subject depends on
  std::shared_ptr<MockWorkSpaceModel> model;
  std::shared_ptr<MockWorkSpaceView> view;
  MockPfTriadManager triad_manager;

  ModelViewMockGenerator<MockWorkModel, MockWorkView> work_mv_generator;
  // endregion

  // region: test subject
  std::shared_ptr<WorkSpacePresenter> presenter;
  // endregion

  // region: object depends on test subject
  SlotCatcher<IWorkSpaceView::UserClickAddWorkSlotType> userClickAddWork;
  SlotCatcher<IWorkSpaceView::UserCloseWorkSlotType> userCloseWork;

  SlotCatcher<IWorkSpaceModel::WorkModelAddedSlotType> workModelAdded;
  SlotCatcher<IWorkSpaceModel::ActiveWorkModelChangedSlotType>
  activeWorkModelChanged;

  SlotCatcher<IWorkSpaceModel::WorkModelActivelyRemovedSlotType>
  workModelActivelyRemoved;
  // endregion
};

TEST_F(WorkSpacePresenterTest, should_create_new_WorkModel_in_model_when_UserClickAddWork) { // NOLINT
  // Setup fixture
  auto work_name = xtestutils::genRandomString();

  // Expectations
  EXPECT_CALL(*model, createWork(work_name));

  // Exercise system
  userClickAddWork(work_name);
}

void WorkSpacePresenterTest::createWorkTriad(CreateWorkTriadResult* result) {
  // Setup fixture
  auto mvpair = work_mv_generator.make_mvpair();
  auto work_model = mvpair->shared_model();
  auto work_view = mvpair->shared_view();
  std::shared_ptr<IPfModel> work_pfmodel = work_model;

  auto work_name = xtestutils::genRandomString();
  ON_CALL(*work_model, name()).WillByDefault(Return(work_name));

  auto aboutToDestroyModel = utils::make_unique<AboutToDestroyModelSlot>();
  auto aboutToDestroyView = utils::make_unique<AboutToDestroyViewSlot>();
  auto workBasicInfoChanged = utils::make_unique<WorkBasicInfoChangedSlot>();

  // Expectations
  EXPECT_CALL(triad_manager, createViewFor(work_pfmodel, _, _))
      .WillOnce(Return(work_view));
  EXPECT_CALL(*view, addWorkView(work_view.get(), work_name));
  EXPECT_CALL(*view, setActiveWorkView(work_view.get()));

  EXPECT_CALL(triad_manager, whenAboutToDestroyModel(work_pfmodel.get(), _, _))
      .WillOnce(SaveArg<1>(aboutToDestroyModel.get()));

  EXPECT_CALL(triad_manager, whenAboutToDestroyView(work_view.get(), _, _))
      .WillOnce(SaveArg<1>(aboutToDestroyView.get()));

  EXPECT_CALL(*work_model, whenBasicInfoChanged(_, _))
      .WillOnce(SaveArg<0>(workBasicInfoChanged.get()));

  // Exercise system
  workModelAdded(work_model);

  // Verify result
  ::Mock::VerifyAndClear(&triad_manager);
  ::Mock::VerifyAndClear(view.get());

  if (result) {
    result->model = work_model.get();
    result->view = work_view.get();
    result->aboutToDestroyModelSlot = std::move(aboutToDestroyModel);
    result->aboutToDestroyViewSlot = std::move(aboutToDestroyView);
    result->workBasicInfoChanged = std::move(workBasicInfoChanged);
  }
}

TEST_F(WorkSpacePresenterTest,
       should_create_WorkView_and_add_as_active_subview_to_WorkSpaceView_when_new_WorkModel_added) { // NOLINT
  CUSTOM_ASSERT(createWorkTriad());
}

TEST_F(WorkSpacePresenterTest, should_change_active_WorkView_when_active_WorkModel_changed) { // NOLINT
  // Setup fixture
  MockWorkModel wm;
  MockWorkView wv;

  auto work_model = &wm;
  auto work_view = &wv;

  // Expectations
  std::vector<IPfView*> views;
  views.push_back(work_view);
  EXPECT_CALL(triad_manager, findViewByModel(work_model))
      .WillOnce(Return(views));
  EXPECT_CALL(*view, setActiveWorkView(work_view));

  // Exercise system
  activeWorkModelChanged(work_model);
}

TEST_F(WorkSpacePresenterTest, should_destroy_Work_triad_when_UserCloseWork) { // NOLINT
  // Setup fixture
  auto work_view = xtestutils::genDummyPointer<MockWorkView>();

  // Expectations
  EXPECT_CALL(triad_manager, removeTriadBy(work_view));

  // Exercise system
  userCloseWork(work_view);
}

TEST_F(WorkSpacePresenterTest,
       should_remove_WorkModel_from_model_when_triad_manager_notifies_WorkModel_is_AboutToDestroy) { // NOLINT
  // Setup fixture
  CreateWorkTriadResult create_result;
  createWorkTriad(&create_result);

  auto work_model = create_result.model;
  auto aboutToDestroyModel = std::move(create_result.aboutToDestroyModelSlot);

  // Expectations
  EXPECT_CALL(*model, removeWorkModel(work_model));

  // Exercise system
  (*aboutToDestroyModel)(work_model);
}

TEST_F(WorkSpacePresenterTest,
       should_remove_WorkView_from_workspace_view_when_WorkView_is_AboutToDestroy) { // NOLINT
  // Setup fixture
  CreateWorkTriadResult create_result;
  createWorkTriad(&create_result);

  auto work_view = create_result.view;
  auto aboutToDestroyView = std::move(create_result.aboutToDestroyViewSlot);

  // Expectations
  EXPECT_CALL(*view, removeWorkView(work_view));

  // Exercise system
  (*aboutToDestroyView)(work_view);
}

TEST_F(WorkSpacePresenterTest, should_remove_Work_triad_when_model_actively_remove_WorkModel) { // NOLINT
  auto work_model = xtestutils::genDummyPointer<IWorkModel>();

  // Expectations
  EXPECT_CALL(triad_manager, removeTriadBy(work_model));

  // Exercise system
  workModelActivelyRemoved(work_model);
}

TEST_F(WorkSpacePresenterTest, should_update_work_tab_title_when_work_name_changed) { // NOLINT
  // Setup fixture
  CreateWorkTriadResult create_result;
  createWorkTriad(&create_result);

  auto work_model = create_result.model;
  auto work_view = create_result.view;
  auto workBasicInfoChanged = std::move(create_result.workBasicInfoChanged);
  auto work_name = work_model->name();

  // Expectations
  EXPECT_CALL(*view, updateWorkViewTitle(work_view, work_name));

  // Exercise system
  (*workBasicInfoChanged)();
}
