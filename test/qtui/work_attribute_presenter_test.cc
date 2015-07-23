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
#include "snail/mock_attribute_display_block.h"

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
    model = std::make_shared<MockWorkAttributeModel>();
    view = std::make_shared<MockWorkAttributeView>();

    IAttributeDisplayBlockVisitor* attr_visitor { nullptr };

    // Expectations
    RECORD_USED_MOCK_OBJECTS_SETUP;

    R_EXPECT_CALL(*model, traverseAttributes(_))
        .WillOnce(SaveArg<0>(&attr_visitor));

    R_EXPECT_CALL(*view, whenEditModeButtonClicked(_, _))
        .WillOnce(SaveArg<0>(&editModeButtonClicked));

    R_EXPECT_CALL(*view, whenDoneButtonClicked(_, _))
        .WillOnce(SaveArg<0>(&doneButtonClicked));

    R_EXPECT_CALL(*model, whenAttributesChanged(_, _))
        .WillOnce(SaveArg<0>(&attributesChanged));

    R_EXPECT_CALL(*model, whenShowPopupFor(_, _))
        .WillOnce(SaveArg<0>(&showPopupFor));

    // Excercise system
    presenter = std::make_shared<WorkAttributePresenter>(
        model, view, &attr_layout);
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;

    ASSERT_EQ(presenter.get(), attr_visitor);
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  std::shared_ptr<MockWorkAttributeModel> model;
  std::shared_ptr<MockWorkAttributeView> view;

  MockAttributeLayout attr_layout;

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

TEST_F(WorkAttributePresenterTest,
       should_be_able_to_show_popup_attribute_editor_dialog_for_model) { // NOLINT
  // Setup fixture
  auto attr_model = std::make_shared<MockAttributeModel>();
  std::shared_ptr<IPfModel> attr_pfmodel = attr_model;
  auto attr_view = std::make_shared<MockAttributeView>();

  auto args = AttrCreateViewArgs::getArgs(true);

  // Expectations
  EXPECT_CALL(triad_manager, createViewFor(attr_pfmodel, _, _, args))
      .WillOnce(Return(attr_view));
  EXPECT_CALL(*attr_view, showView(true))
      .WillOnce(Return(true));

  // Exercise system
  ASSERT_TRUE(showPopupFor(attr_model));
}

TEST_F(WorkAttributePresenterTest,
       should_re_traverse_attributes_when_attributes_changed) { // NOLINT
  // Expectations
  EXPECT_CALL(*model, traverseAttributes(presenter.get()));

  // Exercise system
  attributesChanged();
}

///////////////////// IAttributeDisplayBlockVisitor test begin //////////////
TEST_F(WorkAttributePresenterTest,
       should_relay_beginAddAttributeDisplayBlock_to_attr_layout) { // NOLINT
  // Setup fixture
  int total_block_count = std::rand();

  // Expectations
  EXPECT_CALL(attr_layout, beginAddAttributeDisplayBlock(total_block_count));

  // Exercise system
  presenter->beginAddAttributeDisplayBlock(total_block_count);
}

namespace snailcore {

bool operator==(const AttributeGroupDisplayBlock& a,
                const AttributeGroupDisplayBlock& b) {
  return (a.label == b.label) &&
      (a.add_command == b.add_command) &&
      (a.sub_attr_count == b.sub_attr_count);
}

bool operator==(const AttributeDisplayBlock& a,
                const AttributeDisplayBlock& b) {
  return (a.label == b.label) &&
      (a.attr_model == b.attr_model) &&
      (a.erase_command == b.erase_command) &&
      (a.edit_command == b.edit_command) &&
      (a.is_in_group == b.is_in_group);
}

}  // namespace snailcore


TEST_F(WorkAttributePresenterTest,
       should_relay_AttributeGroupDisplayBlock_to_attr_layout) { // NOLINT
  // Setup fixture
  AttributeGroupDisplayBlock attr_group_block;

  // Expectations
  EXPECT_CALL(attr_layout,
              addAttributeGroupDisplayBlock(attr_group_block));

  // Exercise system
  presenter->addAttributeGroupDisplayBlock(attr_group_block);
}

bool operator==(const AttributeViewDisplayBlock& a,
                const AttributeViewDisplayBlock& b) {
  return (a.label == b.label) &&
      (a.attr_view == b.attr_view) &&
      (a.erase_command == b.erase_command) &&
      (a.edit_command == b.edit_command) &&
      (a.is_in_group == b.is_in_group);
}

TEST_F(WorkAttributePresenterTest,
       should_createViewFor_attr_model_before_add_to_attr_layout) { // NOLINT
  // Setup fixture
  auto attr_model = std::make_shared<MockAttributeModel>();
  std::shared_ptr<IPfModel> attr_pfmodel = attr_model;
  auto attr_view = std::make_shared<MockAttributeView>();

  AttributeDisplayBlock attr_block;
  AttributeViewDisplayBlock attr_view_block;

  attr_block.label = xtestutils::genRandomString();
  attr_block.attr_model = attr_model;
  attr_block.erase_command = xtestutils::genDummyPointer<utils::Command>();
  attr_block.edit_command = xtestutils::genDummyPointer<utils::Command>();
  attr_block.is_in_group = xtestutils::randomBool();

  attr_view_block.label = attr_block.label;
  attr_view_block.attr_view = attr_view.get();
  attr_view_block.erase_command = attr_block.erase_command;
  attr_view_block.edit_command = attr_block.edit_command;
  attr_view_block.is_in_group = attr_block.is_in_group;

  bool edit_mode = true;
  auto args = AttrCreateViewArgs::getArgs(edit_mode);

  // Expectations
  EXPECT_CALL(*model, isEditMode())
      .WillRepeatedly(Return(edit_mode));
  EXPECT_CALL(triad_manager, findViewByModel_if(attr_pfmodel.get(), _))
      .WillOnce(Return(std::vector<IPfView*>()));
  EXPECT_CALL(triad_manager, createViewFor(attr_pfmodel, _, _, args))
      .WillOnce(Return(attr_view));

  EXPECT_CALL(attr_layout, addAttributeDisplayBlock(attr_view_block));

  // Exercise system
  presenter->addAttributeDisplayBlock(attr_block);
}

TEST_F(WorkAttributePresenterTest,
       should_relay_endAddAttributeDisplayBlock_to_attr_layout) { // NOLINT
  // Expectations
  EXPECT_CALL(attr_layout, endAddAttributeDisplayBlock());

  // Exercise system
  presenter->endAddAttributeDisplayBlock();
}

///////////////////// IAttributeDisplayBlockVisitor test end ////////////////
