//-*- TestCaseName: AttributeCollectionPresenterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "pfmvp/mock_pf_triad_manager.h"

// triad headers
#include "snail/mock_attribute_collection_model.h"
#include "qtui/mock_work_attribute_view.h"
#include "src/qtui/attribute_collection_presenter.h"

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

template <typename TestBase>
class AttributeCollectionPresenterTestBase : public TestBase {
 protected:
  AttributeCollectionPresenterTestBase() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeCollectionPresenterTestBase() { }
  void SetUp() override {
    // Setup fixture
    model = std::make_shared<MockAttributeCollectionModel>();
    view = std::make_shared<MockAttributeCollectionView>();

    IAttributeDisplayBlockVisitor* attr_visitor { nullptr };

    // Expectations
    RECORD_USED_MOCK_OBJECTS_SETUP;

    bool initial_mode = false;

    R_EXPECT_CALL(*model, isEditMode())
        .WillOnce(Return(initial_mode));
    R_EXPECT_CALL(*view, setEditMode(initial_mode));
    EXPECT_CALL(attr_layout, set_triad_manager(&triad_manager));

    R_EXPECT_CALL(*model, traverseAttributes(_))
        .WillOnce(SaveArg<0>(&attr_visitor));

    R_EXPECT_CALL(*view, whenEditModeButtonClicked(_, _))
        .WillOnce(SaveArg<0>(&editModeButtonClicked));

    R_EXPECT_CALL(*view, whenDoneButtonClicked(_, _))
        .WillOnce(SaveArg<0>(&doneButtonClicked));

    R_EXPECT_CALL(*model, whenAttributesChanged(_, _))
        .WillOnce(SaveArg<0>(&attributesChanged));

    R_EXPECT_CALL(*model, whenAttrLabelChanged(_, _))
        .WillOnce(SaveArg<0>(&attrLabelChanged));

    // Excercise system
    presenter = std::make_shared<AttributeCollectionPresenter>(
        model, view, &attr_layout);
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;

    ASSERT_EQ(presenter.get(), attr_visitor);
  }
  // void TearDown() override { }

  // region: objects test subject depends on
  std::shared_ptr<MockAttributeCollectionModel> model;
  std::shared_ptr<MockAttributeCollectionView> view;

  MockAttributeLayout attr_layout;

  MockPfTriadManager triad_manager;
  // endregion

  // region: test subject
  std::shared_ptr<AttributeCollectionPresenter> presenter;
  // endregion

  // region: object depends on test subject
  SlotCatcher<IAttributeCollectionView::EditModeButtonClickedSlotType>
  editModeButtonClicked;

  using DoneButtonClickedSlotType =
      IAttributeCollectionView::DoneButtonClickedSlotType;
  SlotCatcher<DoneButtonClickedSlotType> doneButtonClicked;

  using AttributesChangedSlotType =
      IAttributeCollectionModel::AttributesChangedSlotType;
  SlotCatcher<AttributesChangedSlotType> attributesChanged;

  using AttrLabelChangedSlotType =
      IAttributeCollectionModel::AttrLabelChangedSlotType;
  SlotCatcher<AttrLabelChangedSlotType> attrLabelChanged;
  // endregion
};

class AttributeCollectionPresenterTest
    : public AttributeCollectionPresenterTestBase<::testing::Test> { };

TEST_F(AttributeCollectionPresenterTest,
       should_switch_model_to_edit_mode_when_user_click_EditMode_button) { // NOLINT
  // Expectations

  EXPECT_CALL(*model, switchToEditMode());

  // Exercise system
  editModeButtonClicked();
}

TEST_F(AttributeCollectionPresenterTest,
       should_switch_model_to_display_mode_when_user_click_Done_button) { // NOLINT
  // Expectations
  EXPECT_CALL(*model, switchToDisplayMode());

  // Exercise system
  doneButtonClicked();
}

TEST_F(AttributeCollectionPresenterTest,
       should_re_traverse_attributes_when_attributes_changed) { // NOLINT
  // Expectations
  EXPECT_CALL(*model, traverseAttributes(presenter.get()));

  // Exercise system
  attributesChanged();
}

///////////////////// IAttributeDisplayBlockVisitor test begin //////////////
TEST_F(AttributeCollectionPresenterTest,
       should_relay_beginAddAttributeDisplayBlock_to_attr_layout) { // NOLINT
  // Setup fixture
  int total_block_count = std::rand();

  // Expectations
  EXPECT_CALL(attr_layout, beginLayout(total_block_count));

  // Exercise system
  presenter->beginTraverse(total_block_count);
}

TEST_F(AttributeCollectionPresenterTest,
       should_relay_AttributeGroupDisplayBlock_to_attr_layout) { // NOLINT
  // Setup fixture
  AttributeGroupDisplayBlock attr_group_block;

  // Expectations
  EXPECT_CALL(attr_layout,
              layoutAttributeGroupDisplayBlock(attr_group_block));

  // Exercise system
  presenter->visitAttributeGroupDisplayBlock(attr_group_block);
}

bool operator==(const AttributeViewDisplayBlock& a,
                const AttributeViewDisplayBlock& b) {
  return (a.label == b.label) &&
      (a.attr_view == b.attr_view) &&
      (a.is_in_group == b.is_in_group) &&
      (a.view_priv_data == b.view_priv_data);
}

class AttributeCollectionPresenterTest_data_EditMode
    : public AttributeCollectionPresenterTestBase<::testing::TestWithParam<bool>> { }; // NOLINT

INSTANTIATE_TEST_CASE_P(AllEditModes,
                        AttributeCollectionPresenterTest_data_EditMode,
                        ::testing::Bool());

TEST_P(AttributeCollectionPresenterTest_data_EditMode,
       should_createViewFor_attr_model_before_add_to_attr_layout) { // NOLINT
  // Setup fixture
  auto attr_model = std::make_shared<MockAttributeModel>();
  std::shared_ptr<IPfModel> attr_pfmodel = attr_model;
  auto attr_view = std::make_shared<MockAttributeView>();

  AttributeDisplayBlock attr_block;
  AttributeViewDisplayBlock attr_view_block;

  attr_block.label = xtestutils::genRandomString();
  attr_block.attr_model = attr_model;
  attr_block.is_in_group = xtestutils::randomBool();
  attr_block.view_priv_data = xtestutils::genDummyPointer<void>();

  attr_view_block.label = attr_block.label;
  attr_view_block.attr_view = attr_view.get();
  attr_view_block.is_in_group = attr_block.is_in_group;
  attr_view_block.view_priv_data = attr_block.view_priv_data;

  bool edit_mode = GetParam();
  attr_block.edit_mode = edit_mode;
  auto args = AttrCreateViewArgs::getArgs(edit_mode);

  // Expectations
  EXPECT_CALL(triad_manager, findViewByModel_if(attr_pfmodel.get(), _))
      .WillOnce(Return(std::vector<IPfView*>()));
  EXPECT_CALL(triad_manager, createViewFor(attr_pfmodel, _, _, args))
      .WillOnce(Return(attr_view));

  EXPECT_CALL(attr_layout, layoutAttributeDisplayBlock(attr_view_block));

  // Exercise system
  presenter->visitAttributeDisplayBlock(attr_block);
}

TEST_F(AttributeCollectionPresenterTest,
       should_update_label_in_layout_when_LabelChanged) { // NOLINT
  // Setup fixture
  UpdateAttrLabelData label_data;
  label_data.label = xtestutils::genRandomString();
  label_data.view_priv_data = xtestutils::genDummyPointer<void>();

  // Expectations
  EXPECT_CALL(attr_layout, updateLabel(label_data));

  // Exercise system
  attrLabelChanged(label_data);
}

class AttributeCollectionPresenterTest_data_remove_triads
    : public AttributeCollectionPresenterTestBase<::testing::TestWithParam<bool>> { }; // NOLINT

INSTANTIATE_TEST_CASE_P(AllEditModes,
                        AttributeCollectionPresenterTest_data_remove_triads,
                        ::testing::Bool());

TEST_P(AttributeCollectionPresenterTest_data_remove_triads,
       should_relay_endAddAttributeDisplayBlock_to_attr_layout) { // NOLINT
  // Expectations
  bool remove_triads = GetParam();
  EXPECT_CALL(attr_layout, endLayout(remove_triads));

  // Exercise system
  presenter->endTraverse(remove_triads);
}

///////////////////// IAttributeDisplayBlockVisitor test end ////////////////
