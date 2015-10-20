//-*- TestCaseName: KbNodeLinkAttributePopupEditorPresenterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "pfmvp/mock_pf_triad_manager.h"
#include "test/testutils/qmodelindex_generator.h"

// triad headers
#include "snail/mock_kbnode_link_attribute_popup_editor_model.h"
#include "qtui/mock_kbnode_link_attribute_popup_editor_view.h"
#include "src/qtui/core/kbnode_link_attribute_popup_editor_presenter.h"

#include "qtui/mock_tree_item_qmodel.h"
#include "snail/mock_attribute_model.h"
#include "qtui/mock_attribute_editor_view.h"
#include "snail/mock_attribute_set_model.h"
#include "qtui/mock_attribute_set_view.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;  // NOLINT
using namespace pfmvp::tests;  // NOLINT

class KbNodeLinkAttributePopupEditorPresenterTest : public ::testing::Test {
 protected:
  KbNodeLinkAttributePopupEditorPresenterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeLinkAttributePopupEditorPresenterTest() { }
  virtual void SetUp() {
    // Setup fixture
    model = std::make_shared<MockKbNodeLinkAttributePopupEditorModel>();
    view = std::make_shared<MockKbNodeLinkAttributePopupEditorView>();
    auto link_type_qmodel_up = utils::make_unique<MockTreeItemQModel>();
    link_type_qmodel = link_type_qmodel_up.get();

    // Expectations
    RECORD_USED_MOCK_OBJECTS_SETUP;

    createValueAttributeView(&mock_obj_recorder);
    initLinkTypeDropDownList(&mock_obj_recorder);
    createLinkAttributesView(&mock_obj_recorder);

    R_EXPECT_CALL(*view, whenUserSelectLinkType(_, _))
        .WillOnce(SaveArg<0>(&userSelectLinkType));

    R_EXPECT_CALL(*model, whenLinkTypeChanged(_, _))
        .WillOnce(SaveArg<0>(&linkTypeChanged));

    R_EXPECT_CALL(*model, whenValidateComplete(_, _))
        .WillOnce(SaveArg<0>(&validateComplete));

    R_EXPECT_CALL(*view, whenUserClickDone(_, _))
        .WillOnce(SaveArg<0>(&userClickDone));

    // Excercise system
    presenter = std::make_shared<KbNodeLinkAttributePopupEditorPresenter>(
        model, view, std::move(link_type_qmodel_up));
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;
    checkCreateAttrSetViewArgs();
  }
  // virtual void TearDown() { }

  void createValueAttributeView(MockObjectRecorder* mock_recorder);
  void initLinkTypeDropDownList(MockObjectRecorder* mock_recorder);
  void checkCreateAttrSetViewArgs();
  void createLinkAttributesView(MockObjectRecorder* mock_recorder);

  // region: objects test subject depends on
  std::shared_ptr<MockKbNodeLinkAttributePopupEditorModel> model;
  std::shared_ptr<MockKbNodeLinkAttributePopupEditorView> view;

  MockPfTriadManager triad_manager;

  PfCreateViewArgs create_attr_set_view_args;

  MockTreeItemQModel* link_type_qmodel;
  QModelIndexGenerator index_generator;
  // endregion

  // region: test subject
  std::shared_ptr<KbNodeLinkAttributePopupEditorPresenter> presenter;
  // endregion

  // region: object depends on test subject
  using UserSelectLinkTypeSlotType =
      IKbNodeLinkAttributePopupEditorView::UserSelectLinkTypeSlotType;
  SlotCatcher<UserSelectLinkTypeSlotType> userSelectLinkType;

  using LinkTypeChangedSlotType =
      IKbNodeLinkAttributePopupEditorModel::LinkTypeChangedSlotType;
  SlotCatcher<LinkTypeChangedSlotType> linkTypeChanged;

  using UserClickDoneSlotType =
      IKbNodeLinkAttributePopupEditorView::UserClickDoneSlotType;
  SlotCatcher<UserClickDoneSlotType> userClickDone;

  using ValidateCompleteSlotType =
      IKbNodeLinkAttributePopupEditorModel::ValidateCompleteSlotType;
  SlotCatcher<ValidateCompleteSlotType> validateComplete;
  // endregion
};

void KbNodeLinkAttributePopupEditorPresenterTest::checkCreateAttrSetViewArgs() {
  ASSERT_EQ(PF_VIEW_FACTORY(AttributeSetViewNonSwitchableFactory),
            create_attr_set_view_args.view_factory_id());
}

void KbNodeLinkAttributePopupEditorPresenterTest::createValueAttributeView(
    MockObjectRecorder* mock_recorder) {
  MockObjectRecorder& mock_obj_recorder = *mock_recorder;

  auto value_attr_model = std::make_shared<MockAttributeModel>();
  std::shared_ptr<IPfModel> value_attr_pfmodel = value_attr_model;
  auto value_attr_view = std::make_shared<MockAttributeEditorView>();

  R_EXPECT_CALL(*model, createValueKbNodeAttrModel())
      .WillOnce(Return(value_attr_model));
  R_EXPECT_CALL(triad_manager, createViewFor(value_attr_pfmodel, _, _, _))
      .WillOnce(Return(value_attr_view));
  R_EXPECT_CALL(*view, setValueAttrEditor(value_attr_view.get()));
}

void KbNodeLinkAttributePopupEditorPresenterTest::initLinkTypeDropDownList(
    MockObjectRecorder* mock_recorder) {
  MockObjectRecorder& mock_obj_recorder = *mock_recorder;

  auto link_type_provider = xtestutils::genDummyPointer<ITreeItemProvider>();
  R_EXPECT_CALL(*model, getLinkTypeProvider())
      .WillOnce(Return(link_type_provider));
  R_EXPECT_CALL(*link_type_qmodel, setTreeItemProvider(link_type_provider));
  R_EXPECT_CALL(*view, setLinkTypeQModel(link_type_qmodel));

  // select the current link_type
  auto current_link_type = xtestutils::genDummyPointer<IKbNode>();
  auto index = index_generator.index();
  R_EXPECT_CALL(*model, getCurrentLinkType())
      .WillOnce(Return(current_link_type));
  R_EXPECT_CALL(*link_type_qmodel, itemToIndex(current_link_type))
      .WillOnce(Return(index));
  R_EXPECT_CALL(*view, setCurrentLinkType(index));
}

constexpr int kCreateViewArgsIdx = 3;

void KbNodeLinkAttributePopupEditorPresenterTest::createLinkAttributesView(
    MockObjectRecorder* mock_recorder) {
  MockObjectRecorder& mock_obj_recorder = *mock_recorder;

  auto attr_set_model = std::make_shared<MockAttributeSetModel>();
  std::shared_ptr<IPfModel> attr_set_pfmodel = attr_set_model;
  auto attr_set_view = std::make_shared<MockAttributeSetView>();

  R_EXPECT_CALL(*model, getCurrentLinkAttrSetModel())
      .WillOnce(Return(attr_set_model));
  R_EXPECT_CALL(triad_manager, createViewFor(attr_set_pfmodel, _, _, _))
      .WillOnce(DoAll(
          SaveArgPointee<kCreateViewArgsIdx>(&create_attr_set_view_args),
          Return(attr_set_view)));
  R_EXPECT_CALL(*view, setLinkAttributeSetView(attr_set_view.get()));
}

TEST_F(KbNodeLinkAttributePopupEditorPresenterTest,
       should_be_able_to_set_user_select_link_type_to_model) { // NOLINT
  // Setup fixture
  auto link_type = xtestutils::genDummyPointer<IKbNode>();
  auto index = index_generator.index();

  // Expectations
  EXPECT_CALL(*link_type_qmodel, indexToItem(index))
      .WillOnce(Return(link_type));
  EXPECT_CALL(*model, setLinkType(link_type));

  // Exercise system
  userSelectLinkType(index);
}

TEST_F(KbNodeLinkAttributePopupEditorPresenterTest,
       should_re_create_link_attr_set_view_when_link_type_changed) { // NOLINT
  // Setup fixture
  auto attr_set_model = std::make_shared<MockAttributeSetModel>();
  std::shared_ptr<IPfModel> attr_set_pfmodel = attr_set_model;
  auto attr_set_view = std::make_shared<MockAttributeSetView>();

  // Expectations
  EXPECT_CALL(triad_manager, createViewFor(attr_set_pfmodel, _, _, _))
      .WillOnce(DoAll(
          SaveArgPointee<kCreateViewArgsIdx>(&create_attr_set_view_args),
          Return(attr_set_view)));
  EXPECT_CALL(*view, setLinkAttributeSetView(attr_set_view.get()));

  // Exercise system
  linkTypeChanged(attr_set_model);

  // Verify result
  checkCreateAttrSetViewArgs();
}

TEST_F(KbNodeLinkAttributePopupEditorPresenterTest,
       should_set_validate_result_to_view) { // NOLINT
  // Setup fixture
  auto result = xtestutils::randomBool();

  // Expectations
  EXPECT_CALL(*view, setDoneButtonEnabled(result));

  // Exercise system
  validateComplete(result);
}

TEST_F(KbNodeLinkAttributePopupEditorPresenterTest,
       should_tell_model_when_user_click_done) { // NOLINT
  // Expectations
  EXPECT_CALL(*model, editFinished());

  // Exercise system
  userClickDone();
}
