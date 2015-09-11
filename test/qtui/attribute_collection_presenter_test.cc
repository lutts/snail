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
#include "qtui/mock_attribute_collection_view.h"
#include "src/qtui/attribute_collection_presenter.h"

#include "qtui/mock_attribute_collection_qmodel.h"
#include "qtui/mock_attribute_delegate.h"

#include "snail/mock_attribute_model.h"
#include "qtui/mock_attribute_editor_view.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;  // NOLINT
using namespace pfmvp::tests;  // NOLINT

class AttributeCollectionPresenterTest : public ::testing::Test {
 protected:
  AttributeCollectionPresenterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeCollectionPresenterTest() { }
  virtual void SetUp() {
    // Setup fixture
    model = std::make_shared<MockAttributeCollectionModel>();
    view = std::make_shared<MockAttributeCollectionView>();
    qmodel = utils::make_trackable<MockAttributeCollectionQModel>();

    auto attr_delegate_up = utils::make_unique<MockAttributeDelegate>();
    attr_delegate = attr_delegate_up.get();

    // Expectations
    RECORD_USED_MOCK_OBJECTS_SETUP;

    // should init qmodel with supplier list
    std::vector<IAttributeSupplier*> attr_suppliers;
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());

    R_EXPECT_CALL(*model, getAttributeSuppliers())
        .WillOnce(Return(attr_suppliers));
    R_EXPECT_CALL(*qmodel, setAttributeSuppliers(attr_suppliers));

    // should set qmodel to view
    R_EXPECT_CALL(*view, setQModel(qmodel.get()));

    // should set attr delegate to view
    {
      InSequence seq;

      R_EXPECT_CALL(*attr_delegate, whenCreateEditorFor(_, _))
          .WillOnce(SaveArg<0>(&createEditorSignal));

      R_EXPECT_CALL(*attr_delegate, whenCloseEditor(_, _))
          .WillOnce(SaveArg<0>(&closeEditorSignal));

      R_EXPECT_CALL(*view, setAttributeDelegate(attr_delegate));
    }

    // TODO(lutts): may be just a toggleMode?
    R_EXPECT_CALL(*view, whenSwitchToEditMode(_, _))
        .WillOnce(SaveArg<0>(&switchToEditMode));

    R_EXPECT_CALL(*view, whenSwitchToDisplayMode(_, _))
        .WillOnce(SaveArg<0>(&switchToDisplayMode));

    R_EXPECT_CALL(*view, whenUserMayClickAddAttribute(_, _))
        .WillOnce(SaveArg<0>(&userMayClickAddAttribute));

    R_EXPECT_CALL(*qmodel, whenAttributeAdded(_, _))
        .WillOnce(SaveArg<0>(&attributeAdded));

    // Excercise system
    presenter = std::make_shared<AttributeCollectionPresenter>(
        model, view, qmodel, std::move(attr_delegate_up));
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  std::shared_ptr<MockAttributeCollectionModel> model;
  std::shared_ptr<MockAttributeCollectionView> view;

  std::shared_ptr<MockAttributeCollectionQModel> qmodel;
  MockAttributeDelegate* attr_delegate;

  MockPfTriadManager triad_manager;
  // endregion

  // region: test subject
  std::shared_ptr<AttributeCollectionPresenter> presenter;
  // endregion

  // region: object depends on test subject
  SlotCatcher<IAttributeDelegate::CreateEditorForSlotType> createEditorSignal;
  SlotCatcher<IAttributeDelegate::CloseEditorSlotType> closeEditorSignal;

  using SwitchToEditModeSlotType =
      IAttributeCollectionView::SwitchToEditModeSlotType;
  SlotCatcher<SwitchToEditModeSlotType> switchToEditMode;

  using SwitchToDisplayModeSlotType =
      IAttributeCollectionView::SwitchToDisplayModeSlotType;
  SlotCatcher<SwitchToDisplayModeSlotType> switchToDisplayMode;

  using UserMayClickAddAttributeSlotType =
      IAttributeCollectionView::UserMayClickAddAttributeSlotType;
  SlotCatcher<UserMayClickAddAttributeSlotType> userMayClickAddAttribute;

  using AttributeAddedSlotType =
      IAttributeCollectionQModel::AttributeAddedSlotType;
  SlotCatcher<AttributeAddedSlotType> attributeAdded;
  // endregion
};

TEST_F(AttributeCollectionPresenterTest, should_construct_properly) { // NOLINT
  // See SetUp()
  SUCCEED();
}

TEST_F(AttributeCollectionPresenterTest,
       should_be_able_to_create_attr_editor_view_for_attr_delegate) { // NOLINT
  // Setup fixture
  int row = std::rand();
  IAttribute* dummy_attr = xtestutils::genDummyPointer<IAttribute>();
  auto attr_model = std::make_shared<MockAttributeModel>();
  std::shared_ptr<IPfModel> attr_pfmodel = attr_model;
  auto attr_editor_view = std::make_shared<MockAttributeEditorView>();

  // Expectations
  EXPECT_CALL(*qmodel, attrOfRow(row))
      .WillOnce(Return(dummy_attr));
  EXPECT_CALL(*model, createAttributeModel(dummy_attr))
      .WillOnce(Return(attr_model));
  // TODO(lutts): create with args
  EXPECT_CALL(triad_manager, createViewFor(attr_pfmodel, _, _, _))
      .WillOnce(Return(attr_editor_view));

  // Exercise system
  auto actual_editor_view = createEditorSignal(row);

  // Verify results
  ASSERT_EQ(attr_editor_view.get(), actual_editor_view);
}

TEST_F(AttributeCollectionPresenterTest,
       should_not_create_editor_view_for_supplier_rows) { // NOLINT
  // Setup fixture
  int row = std::rand();

  // Expectations
  EXPECT_CALL(*qmodel, attrOfRow(row)).WillOnce(Return(nullptr));
  EXPECT_CALL(*model, createAttributeModel(_)).Times(0);
  EXPECT_CALL(triad_manager, createViewFor(_, _, _, _)).Times(0);

  // Exercise system
  auto actual_editor_view = createEditorSignal(row);

  // Verify results
  ASSERT_EQ(nullptr, actual_editor_view);
}

TEST_F(AttributeCollectionPresenterTest,
       should_be_able_to_remove_attr_editor_triad_when_close_editor) { // NOLINT
  // Setup fixture
  auto attr_editor_view = xtestutils::genDummyPointer<IAttributeEditorView>();

  // Expectations
  EXPECT_CALL(triad_manager, removeTriadBy(attr_editor_view));

  // Exercise system
  closeEditorSignal(attr_editor_view);
}

TEST_F(AttributeCollectionPresenterTest,
       should_tell_qmodel_enter_edit_mode_and_open_attr_editor_views_in_view_when_user_switch_to_edit_mode) { // NOLINT
  // Setup fixture
  int row_count = 3;

  // Expectations
  {
    InSequence seq;

    EXPECT_CALL(*qmodel, switchToEditMode());
    EXPECT_CALL(*qmodel, attrRowCount()).WillOnce(Return(row_count));

    for (int row = 0; row < row_count; ++row) {
      EXPECT_CALL(*view, openAttributeEditor(row));
    }
  }

  // Exercise system
  switchToEditMode();
}

TEST_F(AttributeCollectionPresenterTest,
       should_tell_model_qmodel_to_enter_diplaymode_and_close_attr_editors_when_switch_to_display_mode) { // NOLINT
  // Setup fixture
  int row_count = 3;

  // Expectations
  {
    InSequence seq;

    // first, close editors
    EXPECT_CALL(*qmodel, attrRowCount()).WillOnce(Return(row_count));

    for (int row = 0; row < row_count; ++row) {
      EXPECT_CALL(*view, closeAttributeEditor(row));
    }

    // then operation on the models
    EXPECT_CALL(*qmodel, switchToDisplayMode());
  }

  // Exercise system
  switchToDisplayMode();
}

TEST_F(AttributeCollectionPresenterTest,
       should_try_to_add_attribute_when_user_click_add_attribute) { // NOLINT
  // Setup fixture
  int row = std::rand();

  // Expectations
  EXPECT_CALL(*qmodel, mayAddAttributeIfSupplier(row));

  // Exercise system
  userMayClickAddAttribute(row);
}

TEST_F(AttributeCollectionPresenterTest,
       should_open_editor_for_newly_added_attribute) { // NOLINT
  // Setup fixture
  int row = std::rand();

  // Expectations
  EXPECT_CALL(*view, openAttributeEditor(row));

  // Exercise system
  attributeAdded(row);
}
