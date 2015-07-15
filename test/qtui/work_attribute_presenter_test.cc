//-*- TestCaseName: WorkAttributePresenterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "pfmvp/mock_pf_triad_manager.h"

// triad headers
#include "snail/mock_work_attribute_model.h"
#include "qtui/mock_work_attribute_view.h"
#include "src/qtui/work_attribute_presenter.h"

#include "src/qtui/attr_create_view_args.h"

// triad headers
#include "snail/mock_attribute_model.h"
#include "qtui/mock_attribute_view.h"

#include "utils/command.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;  // NOLINT
using namespace pfmvp::tests;  // NOLINT
using namespace utils;  // NOLINT

class WorkAttributePresenterTest : public ::testing::Test {
 protected:
  WorkAttributePresenterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkAttributePresenterTest() { }
  virtual void SetUp() {
    // Setup fixture
    model = std::make_shared<StrictMock<MockWorkAttributeModel>>();
    view = std::make_shared<StrictMock<MockWorkAttributeView>>();

    // Expectations
    bool init_edit_mode = false;
    expectationsOnReLayoutUI(init_edit_mode);

    R_EXPECT_CALL(*view, whenEditModeButtonClicked(_, _))
        .WillOnce(SaveArg<0>(&editModeButtonClicked));

    R_EXPECT_CALL(*view, whenDoneButtonClicked(_, _))
        .WillOnce(SaveArg<0>(&doneButtonClicked));

    R_EXPECT_CALL(*model, whenAttributesChanged(_, _))
        .WillOnce(SaveArg<0>(&attributesChanged));

    R_EXPECT_CALL(*model, whenShowPopupFor(_, _))
        .WillOnce(SaveArg<0>(&showPopupFor));

    // Excercise system
    presenter = std::make_shared<WorkAttributePresenter>(model, view);
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;
  }
  // virtual void TearDown() { }

  void expectationsOnReLayoutUI(bool edit_mode);
  void expectationsOnShowAttributes(bool edit_mode);
  void expectationsOnShowCommands();

  RECORD_USED_MOCK_OBJECTS_SETUP;

  // region: objects test subject depends on
  std::shared_ptr<StrictMock<MockWorkAttributeModel>> model;
  std::shared_ptr<StrictMock<MockWorkAttributeView>> view;

  MockPfTriadManager triad_manager;
  // endregion

  // region: test subject
  std::shared_ptr<WorkAttributePresenter> presenter;
  // endregion

  // region: object depends on test subject
  SlotCatcher<IWorkAttributeView::EditModeButtonClickedSlotType>
  editModeButtonClicked;

  SlotCatcher<IWorkAttributeView::DoneButtonClickedSlotType> doneButtonClicked;

  SlotCatcher<IWorkAttributeModel::AttributesChangedSlotType> attributesChanged;
  SlotCatcher<IWorkAttributeModel::ShowPopupForSlotType> showPopupFor;
  // endregion
};

TEST_F(WorkAttributePresenterTest,
       should_switch_model_to_edit_mode_when_user_click_EditMode_button) { // NOLINT
  // Expectations
  EXPECT_CALL(*model, switchToEditMode());

  // Exercise system
  editModeButtonClicked();
}

TEST_F(WorkAttributePresenterTest,
       should_switch_model_to_display_mode_when_user_click_Done_button) { // NOLINT
  // Expectations
  EXPECT_CALL(*model, switchToDisplayMode());

  // Exercise system
  doneButtonClicked();
}

void WorkAttributePresenterTest::expectationsOnShowAttributes(
    bool edit_mode) {

  auto args = AttrCreateViewArgs::getArgs(edit_mode);

  const int NUM_ATTR_MODELS = 3;
  std::vector<std::shared_ptr<IAttributeModel>> attr_models;

  for (int i = 0; i < NUM_ATTR_MODELS; ++i) {
    auto attr_model = std::make_shared<MockAttributeModel>();
    ON_CALL(*attr_model, displayName())
        .WillByDefault(Return(xtestutils::genRandomString()));

    attr_models.push_back(attr_model);
  }

  R_EXPECT_CALL(*model, getAttributeModels())
      .WillOnce(Return(attr_models));
  for (auto& attr_model : attr_models) {
    std::shared_ptr<IPfModel> attr_pfmodel = attr_model;
    auto attr_view = std::make_shared<MockAttributeView>();

    R_EXPECT_CALL(triad_manager, findViewByModel_if(attr_pfmodel.get(), _))
        .WillOnce(Return(std::vector<IPfView*>()));
    R_EXPECT_CALL(triad_manager, createViewFor(attr_pfmodel, args))
        .WillOnce(Return(attr_view));

    IWorkAttributeModel::Location location {
      std::rand(), std::rand(), std::rand(), std::rand()
          };

    R_EXPECT_CALL(*model, getLocation(attr_model.get()))
        .WillOnce(Return(location));
    R_EXPECT_CALL(*view, addLabel(attr_model->displayName(),
                                  location.row(),
                                  location.column() - 1,
                                  location.row_span(),
                                  location.column_span()));
    R_EXPECT_CALL(*view, addAttribute(attr_view.get(),
                                      location.row(),
                                      location.column(),
                                      location.row_span(),
                                      location.column_span()));
  }
}

// TODO(lutts.cao@gmail.com 2015-07-15): Maybe we can get command for attributes
void WorkAttributePresenterTest::expectationsOnShowCommands() {
  const int NUM_COMMANDS = 3;

#define SHOW_COMMANDS(CmdType)                                          \
  {                                                                     \
    std::vector<Command*> commands;                                     \
    for (int i = 0; i < NUM_COMMANDS; ++i) {                            \
      commands.push_back(xtestutils::genDummyPointer<Command>());       \
    }                                                                   \
                                                                        \
    EXPECT_CALL(*model, get##CmdType##Commands())                       \
        .WillOnce(Return(commands));                                    \
                                                                        \
    for (auto & command : commands) {                                   \
      IWorkAttributeModel::Location location {                          \
        std::rand(), std::rand(), std::rand(), std::rand()              \
            };                                                          \
                                                                        \
      R_EXPECT_CALL(*model, getLocation(command))                       \
          .WillOnce(Return(location));                                  \
                                                                        \
      EXPECT_CALL(*view, add##CmdType##Command(command,                 \
                                               location.row(),          \
                                               location.column(),       \
                                               location.row_span(),     \
                                               location.column_span())); \
    }                                                                   \
  }

  SHOW_COMMANDS(Erase);
  SHOW_COMMANDS(PopupEditor);
  SHOW_COMMANDS(AddAttribute)
}

void WorkAttributePresenterTest::expectationsOnReLayoutUI(bool edit_mode) {
  EXPECT_CALL(*model, isEditMode())
      .WillRepeatedly(Return(edit_mode));

  InSequence seq;

  EXPECT_CALL(*view, beginReLayoutAttributes());

  expectationsOnShowAttributes(edit_mode);
  if (edit_mode)
    expectationsOnShowCommands();

  EXPECT_CALL(*view, endReLayoutAttributes());
}

TEST_F(WorkAttributePresenterTest,
       should_refresh_ui_when_attributes_changed_in_edit_mode) { // NOLINT
  // Expectations
  expectationsOnReLayoutUI(true);

  // Exercise system
  attributesChanged();
}

TEST_F(WorkAttributePresenterTest,
       should_refresh_ui_when_attributes_changed_in_display_mode) { // NOLINT
  // Expectations
  expectationsOnReLayoutUI(false);

  // Exercise system
  attributesChanged();
}

TEST_F(WorkAttributePresenterTest,
       should_be_able_to_show_popup_attribute_editor_dialog_for_model) { // NOLINT
  // Setup fixture
  auto attr_model = std::make_shared<MockAttributeModel>();
  std::shared_ptr<IPfModel> attr_pfmodel = attr_model;
  auto attr_view = std::make_shared<MockAttributeView>();

  auto args = AttrCreateViewArgs::getArgs(true);

  // Expectations
  EXPECT_CALL(triad_manager, createViewFor(attr_pfmodel, args))
      .WillOnce(Return(attr_view));
  EXPECT_CALL(*attr_view, showView(true))
      .WillOnce(Return(true));

  // Exercise system
  ASSERT_TRUE(showPopupFor(attr_model));
}
