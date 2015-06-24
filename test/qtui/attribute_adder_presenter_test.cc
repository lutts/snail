//-*- TestCaseName: AttributeAdderPresenterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <cstdlib>

#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/utils.h"
#include "test/testutils/generic_mock_listener.h"
#include "test/testutils/slot_catcher.h"
#include "test/testutils/mock_object_generator.h"
#include "test/testutils/model_view_mock_generator.h"
#include "pfmvp/mock_pf_triad_manager.h"

// triad headers
#include "snail/mock_attribute_adder_model.h"
#include "qtui/mock_attribute_adder_dialog.h"
#include "src/qtui/attribute_adder_presenter.h"

#include "snail/mock_attribute.h"
#include "snail/mock_attribute_editor_model.h"
#include "qtui/mock_attribute_editor_view.h"
#include "qtui/mock_attribute_list_qmodel.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;  // NOLINT
using namespace pfmvp::tests;  // NOLINT

class AttributeAdderPresenterTest : public ::testing::Test {
 protected:
  AttributeAdderPresenterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeAdderPresenterTest() { }
  virtual void SetUp() {
    model = std::make_shared<MockAttributeAdderModel>();
    view = std::make_shared<MockAttributeAdderDialog>();

    auto attrListQModel = utils::make_unique<MockAttributeListQModel>();

    // expectations
    // set prompt
    auto dummyPrompt = xtestutils::genRandomString();
    R_EXPECT_CALL(*model, getPrompt()).WillOnce(Return(dummyPrompt));
    R_EXPECT_CALL(*view, setPrompt(dummyPrompt));

    // initialize attribute selector
    // 1. initialize the drop-down list
    std::vector<IAttribute*> allowed_attr_list;
    // add some dummy data to make the list not empty
    MockAttribute dummyAttr1;
    MockAttribute dummyAttr2;
    allowed_attr_list.push_back(&dummyAttr1);
    allowed_attr_list.push_back(&dummyAttr2);

    R_EXPECT_CALL(*model, getAllowedAttributeList())
        .WillOnce(Return(allowed_attr_list));

    {
      InSequence seq;

      // TODO(lutts): this means the presenter is directly access layer under
      // pmodel, is this what we want?
      R_EXPECT_CALL(*attrListQModel, setAttributeList(allowed_attr_list));
      R_EXPECT_CALL(*view, setAttributeListQModel(attrListQModel.get()));
      // 2. set the default selection
      int index = 0xbeaf;
      R_EXPECT_CALL(*model, getCurrentAttributeIndex())
          .WillOnce(Return(index));
      R_EXPECT_CALL(*view, setCurrentAttributeIndex(index));
    }

    // create attribute editor for the current selected attribute
    auto attr_model = std::make_shared<MockAttributeEditorModel>();
    R_EXPECT_CALL(*model, getCurrentAttributeEditorModel())
        .WillOnce(Return(attr_model));
    expectationsOnBuildAttributeEditorView(attr_model);

    // enable done button
    R_EXPECT_CALL(*view, setDoneButtonEnabled(true));

    R_EXPECT_CALL(*view, whenCurrentAttributeIndexChanged(_, _))
        .WillOnce(SaveArg<0>(&currAttrIndexChanged));

    R_EXPECT_CALL(*view, whenAddButtonClicked(_, _))
        .WillOnce(SaveArg<0>(&addButtonClicked));

    R_EXPECT_CALL(*model, whenValidateComplete(_, _))
        .WillOnce(SaveArg<0>(&validateComplete));

    R_EXPECT_CALL(*model, whenDiscardAttributeEditorModel(_, _))
        .WillOnce(SaveArg<0>(&discardAttributeEditorModel));

    R_EXPECT_CALL(*model, whenCurrentAttributeEditorModelChanged(_, _))
        .WillOnce(SaveArg<0>(&currAttrModelChanged));

    presenter = std::make_shared<AttributeAdderPresenter>(
        model, view,
        std::move(attrListQModel));
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;

    ASSERT_EQ(presenter.get(), attr_editor_triad_parent_presenter);
  }
  // virtual void TearDown() { }

  void expectationsOnBuildAttributeEditorView(
      std::shared_ptr<IAttributeEditorModel> attr_model);

  // region: objects test subject depends on
  std::shared_ptr<MockAttributeAdderModel> model;
  std::shared_ptr<MockAttributeAdderDialog> view;

  MockPfTriadManager triad_manager;

  RECORD_USED_MOCK_OBJECTS_SETUP;

  PfPresenter* attr_editor_triad_parent_presenter { nullptr };
  // endregion

  // region: test subject
  std::shared_ptr<AttributeAdderPresenter> presenter;
  // endregion

  // region: object depends on test subject
  using CurrAttrIndexChangedSlotType =
      IAttributeAdderDialog::CurrentAttributeIndexChangedSlotType;
  SlotCatcher<CurrAttrIndexChangedSlotType> currAttrIndexChanged;
  SlotCatcher<IAttributeAdderDialog::AddButtonClickedSlotType> addButtonClicked;

  SlotCatcher<IAttributeAdderModel::ValidateCompleteSlotType> validateComplete;
  SlotCatcher<IAttributeAdderModel::DiscardAttributeEditorModelSlotType>
  discardAttributeEditorModel;

  SlotCatcher<IAttributeAdderModel::CurrentAttributeEditorModelChangedSlotType>
  currAttrModelChanged;
  // endregion
};

void AttributeAdderPresenterTest::expectationsOnBuildAttributeEditorView(
    std::shared_ptr<IAttributeEditorModel> attr_model) {
  auto attr_editor = std::make_shared<MockAttributeEditorView>();

  // reset presenter recorder
  attr_editor_triad_parent_presenter = nullptr;

  std::shared_ptr<IPfModel> attr_pfmodel = attr_model;
  R_EXPECT_CALL(triad_manager, createViewFor(attr_pfmodel, _, true))
      .WillOnce(DoAll(SaveArg<1>(&attr_editor_triad_parent_presenter),
                      Return(attr_editor)));
  R_EXPECT_CALL(*view, setAttributeEditor(attr_editor.get()));

  // disable add button on refresh editor view
  R_EXPECT_CALL(*view, setAddButtonEnabled(false));
}

TEST_F(AttributeAdderPresenterTest,
       should_update_add_button_status_when_validate_complete) { // NOLINT
  auto tester = [this](bool expect_result) {
    // Expectations
    EXPECT_CALL(*view, setAddButtonEnabled(expect_result));

    // Exercise system
    validateComplete(expect_result);

    ::Mock::VerifyAndClear(view.get());
  };

  tester(true);
  tester(false);
}

TEST_F(AttributeAdderPresenterTest,
       should_set_new_attr_index_to_model_when_CurrentAttributeIndexChanged) { // NOLINT
  int index = std::rand();

  // Expectations
  EXPECT_CALL(*model, setCurrentAttributeIndex(index));

  // Exercise system
  currAttrIndexChanged(index);
}

TEST_F(AttributeAdderPresenterTest,
       should_remove_attr_editor_triad_when_discard_attr_model) { // NOLINT
  // Setup fixture
  MockAttributeEditorModel attr_model;
  MockAttributeEditorView attr_editor;

  std::vector<IPfView*> attr_editor_vec;
  attr_editor_vec.push_back(&attr_editor);

  // Expectations
  EXPECT_CALL(triad_manager, findViewByModel(&attr_model))
      .WillOnce(Return(attr_editor_vec));
  EXPECT_CALL(*view, removeAttributeEditor(&attr_editor));
  EXPECT_CALL(triad_manager, removeTriadBy(&attr_model));

  // Exercise system
  discardAttributeEditorModel(&attr_model);
}

TEST_F(AttributeAdderPresenterTest,
       should_build_attr_editor_triad_and_update_view_when_CurrentAttributeEditorModelChanged) { // NOLINT
  // Setup fixture
  auto attr_model = std::make_shared<MockAttributeEditorModel>();

  // Expectations
  expectationsOnBuildAttributeEditorView(attr_model);

  // Exercise system
  currAttrModelChanged(attr_model);

  // Verify result
  ASSERT_EQ(presenter.get(), attr_editor_triad_parent_presenter);
}

TEST_F(AttributeAdderPresenterTest,
       should_tell_model_to_add_attribute_when_AddButtonClicked) { // NOLINT
  // Expectations
  EXPECT_CALL(*model, doAddAttribute());

  // Exercise system
  addButtonClicked();
}
