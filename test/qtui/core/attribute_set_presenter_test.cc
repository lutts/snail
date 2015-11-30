//-*- TestCaseName: AttributeSetPresenterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "pfmvp/mock_pf_triad_manager.h"

// triad headers
#include "snail/mock_attribute_set_model.h"
#include "qtui/ui/mock_attribute_set_view.h"
#include "src/qtui/core/attribute_set_presenter.h"

#include "qtui/core/mock_attribute_set_layout.h"
#include "snail/i_attribute_supplier.h"

#include "snail/mock_attribute_model.h"
#include "qtui/ui/mock_attribute_editor_view.h"

using namespace snailcore;         // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;             // NOLINT
using namespace pfmvp::tests;      // NOLINT

class AttributeSetPresenterTest : public ::testing::Test {
 protected:
  AttributeSetPresenterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeSetPresenterTest() { }
  virtual void SetUp() {
    // Setup fixture
    model = std::make_shared<MockAttributeSetModel>();
    view = std::make_shared<MockAttributeSetView>();
    attr_set_layout = std::make_shared<MockAttributeSetLayout>();

    // Expectations
    RECORD_USED_MOCK_OBJECTS_SETUP;

    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());

    bool edit_mode = xtestutils::randomBool();
    R_EXPECT_CALL(*model, isEditMode()).WillOnce(Return(edit_mode));

    R_EXPECT_CALL(*model, getAttributeSuppliers())
        .WillOnce(Return(attr_suppliers));
    R_EXPECT_CALL(*attr_set_layout,
                  setAttributeSuppliers(attr_suppliers, edit_mode));
    if (edit_mode) {
      EXPECT_CALL(*view, switchToEditMode());
    } else {
      EXPECT_CALL(*view, switchToDisplayMode());
    }

    R_EXPECT_CALL(*view, whenUserSwitchMode(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&userSwitchMode), Return(SignalConnection())));

    R_EXPECT_CALL(*attr_set_layout, whenCreateAttrEditor(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&createAttrEditor), Return(SignalConnection())));

    R_EXPECT_CALL(*attr_set_layout, whenCloseAttributeEditors(_, _))
        .WillOnce(DoAll(SaveArg<0>(&closeAttributeEditors),
                        Return(SignalConnection())));

    R_EXPECT_CALL(*model, whenSwitchToEditMode(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&switchToEditMode), Return(SignalConnection())));

    R_EXPECT_CALL(*model, whenSwitchToDisplayMode(_, _))
        .WillOnce(DoAll(SaveArg<0>(&switchToDisplayMode),
                        Return(SignalConnection())));

    R_EXPECT_CALL(*model, whenValidateComplete(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&validateComplete), Return(SignalConnection())));

    // Excercise system
    presenter =
        std::make_shared<AttributeSetPresenter>(model, view, attr_set_layout);
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  std::shared_ptr<MockAttributeSetModel> model;
  std::shared_ptr<MockAttributeSetView> view;

  std::shared_ptr<MockAttributeSetLayout> attr_set_layout;

  MockPfTriadManager triad_manager;

  std::vector<IAttributeSupplier*> attr_suppliers;
  // endregion

  // region: test subject
  std::shared_ptr<AttributeSetPresenter> presenter;
  // endregion

  // region: object depends on test subject
  using UserSwitchModeSlotType = IAttributeSetView::UserSwitchModeSlotType;
  SlotCatcher<UserSwitchModeSlotType> userSwitchMode;

  using CreateAttrEditorSlotType =
      fto::AttributeSetLayout::CreateAttrEditorSlotType;
  SlotCatcher<CreateAttrEditorSlotType> createAttrEditor;

  using CloseAttributeEditorsSlotType =
      fto::AttributeSetLayout::CloseAttributeEditorsSlotType;
  SlotCatcher<CloseAttributeEditorsSlotType> closeAttributeEditors;

  using SwitchToEditModeSlotType = IAttributeSetModel::SwitchToEditModeSlotType;
  SlotCatcher<SwitchToEditModeSlotType> switchToEditMode;

  using SwitchToDisplayModeSlotType =
      IAttributeSetModel::SwitchToDisplayModeSlotType;
  SlotCatcher<SwitchToDisplayModeSlotType> switchToDisplayMode;

  using ValidateCompleteSlotType = IAttributeSetModel::ValidateCompleteSlotType;
  SlotCatcher<ValidateCompleteSlotType> validateComplete;
  // endregion
};

TEST_F(AttributeSetPresenterTest,
       should_tell_model_when_user_switch_mode) {  // NOLINT
  // Expectations

  EXPECT_CALL(*model, switchMode());

  // Exercise system
  userSwitchMode();
}

TEST_F(
    AttributeSetPresenterTest,
    should_tell_layout_and_view_to_enter_edit_mode_when_model_switch_to_edit_mode) {  // NOLINT
  // Expectations
  EXPECT_CALL(*model, getAttributeSuppliers()).WillOnce(Return(attr_suppliers));
  EXPECT_CALL(*attr_set_layout, setAttributeSuppliers(attr_suppliers, true));
  EXPECT_CALL(*view, switchToEditMode());

  // Exercise system
  switchToEditMode();
}

TEST_F(
    AttributeSetPresenterTest,
    should_tell_layout_and_view_to_enter_display_model_when_model_switch_to_display_mode) {  // NOLINT
  // Expectations
  EXPECT_CALL(*model, getAttributeSuppliers()).WillOnce(Return(attr_suppliers));
  EXPECT_CALL(*attr_set_layout, setAttributeSuppliers(attr_suppliers, false));
  EXPECT_CALL(*view, switchToDisplayMode());

  // Exercise system
  switchToDisplayMode();
}

TEST_F(
    AttributeSetPresenterTest,
    should_ask_model_to_close_created_attr_editor_triads_when_attr_set_layout_request_close) {  // NOLINT
  // Expectations
  EXPECT_CALL(*model, closeAttributeEditors(&triad_manager));

  // Exercise system
  closeAttributeEditors();
}

TEST_F(
    AttributeSetPresenterTest,
    should_be_able_to_create_attr_editors_when_layout_request_create_attr_editor) {  // NOLINT
  // Setup fixture
  IAttribute* attr = xtestutils::genDummyPointer<IAttribute>();
  auto attr_model = std::make_shared<MockAttributeModel>();
  std::shared_ptr<IPfModel> attr_pfmodel = attr_model;
  auto attr_editor_view = std::make_shared<MockAttributeEditorView>();

  // Expectations
  EXPECT_CALL(*model, createAttributeModel(attr)).WillOnce(Return(attr_model));
  EXPECT_CALL(triad_manager, createViewFor(attr_pfmodel, _, _, _))
      .WillOnce(Return(attr_editor_view));

  // Exercise system
  auto actual_editor_view = createAttrEditor(attr);

  // Verify results
  ASSERT_EQ(attr_editor_view.get(), actual_editor_view);
}

TEST_F(
    AttributeSetPresenterTest,
    should_update_switch_mode_button_state_when_model_validate_complete) {  // NOLINT
  // Setup fixture
  auto validate_result = xtestutils::randomBool();

  // Expectations
  EXPECT_CALL(*view, setSwitchModelButtonEnabled(validate_result));

  // Exercise system
  validateComplete(validate_result);
}
