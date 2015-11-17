//-*- TestCaseName: KbNodeAttributeEditPresenterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <functional>

#include "test/testutils/gmock_common.h"
#include "pfmvp/mock_pf_triad_manager.h"
#include "test/testutils/qmodelindex_generator.h"
#include "test/testutils/random_string.h"

// triad headers
#include "snail/mock_kbnode_attribute_model.h"
#include "snail/mock_tree_item_provider.h"
#include "qtui/ui/mock_kbnode_attribute_edit_view.h"
#include "qtui/core/mock_tree_item_qmodel.h"
#include "src/qtui/core/kbnode_attribute_edit_presenter.h"
#include "core/mock_kbnode.h"

#include "snail/mock_simple_kbnode_adder_model.h"
#include "qtui/ui/mock_simple_kbnode_adder_view.h"

using namespace snailcore;         // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;             // NOLINT
using namespace pfmvp::tests;      // NOLINT

class KbNodeAttributeEditPresenterTest : public ::testing::Test {
 protected:
  KbNodeAttributeEditPresenterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeAttributeEditPresenterTest() { }
  virtual void SetUp() {
    // Setup fixture
    model = std::make_shared<MockKbNodeAttributeModel>();
    view = std::make_shared<MockKbNodeAttributeEditView>();
    kbnode_qmodel = std::make_shared<MockTreeItemQModel>();

    // Expectations
    RECORD_USED_MOCK_OBJECTS_SETUP;

    xtestutils::RandomString kbnode_name;

    R_EXPECT_CALL(*model, getKbNodeName()).WillOnce(Return(kbnode_name.ustr()));
    R_EXPECT_CALL(*view, setKbNodeName(kbnode_name.qstr()));

    EXPECT_CALL(*model, getKbNodeProvider())
        .WillRepeatedly(Return(&kbnode_provider));
    {
      InSequence seq;

      R_EXPECT_CALL(*kbnode_qmodel, setTreeItemProvider(&kbnode_provider));

      auto qmodel = xtestutils::genDummyPointer<QAbstractItemModel>();
      R_EXPECT_CALL(*kbnode_qmodel, qmodel()).WillOnce(Return(qmodel));
      R_EXPECT_CALL(*view, setKbNodeTreeQModel(qmodel));
    }

    R_EXPECT_CALL(*view, whenUserClickedIndex(_, _))
        .WillOnce(SaveArg<0>(&userClickedIndex));

    R_EXPECT_CALL(*view, whenFilterPatternChanged(_, _))
        .WillOnce(SaveArg<0>(&filterPatternChanged));

    R_EXPECT_CALL(*view, whenEditingFinished(_, _))
        .WillOnce(SaveArg<0>(&editingFinished));

    R_EXPECT_CALL(*view, whenUserClickAddKbNode(_, _))
        .WillOnce(SaveArg<0>(&userClickAddKbNode));

    // Excercise system
    presenter = std::make_shared<KbNodeAttributeEditPresenter>(model, view,
                                                               kbnode_qmodel);
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;

    EXPECT_CALL(*model, getKbNodeProvider())
        .WillRepeatedly(Return(&kbnode_provider));
    EXPECT_CALL(kbnode_provider, name())
        .WillRepeatedly(Return(provider_name.ustr()));
  }
  // virtual void TearDown() { }

  std::function<void()> expectationOnAddKbNode();

  // region: objects test subject depends on
  std::shared_ptr<MockKbNodeAttributeModel> model;
  std::shared_ptr<MockKbNodeAttributeEditView> view;

  xtestutils::RandomString provider_name;
  MockTreeItemProvider kbnode_provider;
  std::shared_ptr<MockTreeItemQModel> kbnode_qmodel;

  MockPfTriadManager triad_manager;
  QModelIndexGenerator index_generator;
  // endregion

  // region: test subject
  std::shared_ptr<KbNodeAttributeEditPresenter> presenter;
  // endregion

  // region: object depends on test subject
  using UserClickedIndexSlotType =
      IKbNodeAttributeEditView::UserClickedIndexSlotType;
  SlotCatcher<UserClickedIndexSlotType> userClickedIndex;

  using FilterPatternChangedSlotType =
      IKbNodeAttributeEditView::FilterPatternChangedSlotType;
  SlotCatcher<FilterPatternChangedSlotType> filterPatternChanged;

  using EditingFinishedSlotType =
      IKbNodeAttributeEditView::EditingFinishedSlotType;
  SlotCatcher<EditingFinishedSlotType> editingFinished;

  using UserClickAddKbNodeSlotType =
      IKbNodeAttributeEditView::UserClickAddKbNodeSlotType;
  SlotCatcher<UserClickAddKbNodeSlotType> userClickAddKbNode;
  // endregion
};

std::function<void()>
KbNodeAttributeEditPresenterTest::expectationOnAddKbNode() {
  auto kbnode_provider_model = std::make_shared<MockSimpleKbNodeAdderModel>();
  auto add_kbnode_dialog_view = std::make_shared<MockSimpleKbNodeAdderView>();

  {
    InSequence seq;

    EXPECT_CALL(*model, createSimpleKbNodeAdderModel())
        .WillOnce(Return(kbnode_provider_model));

    // the following three are showDialog expectations
    triad_manager.expectationsOnShowModalDialog<MockSimpleKbNodeAdderView>(
        kbnode_provider_model, add_kbnode_dialog_view);
  }

  // Workaround for gmock InSequence hold shared_ptr problem
  auto raw_kbnode_provider_model = kbnode_provider_model.get();
  auto raw_add_kbnode_dialog_view = add_kbnode_dialog_view.get();
  return [raw_kbnode_provider_model, raw_add_kbnode_dialog_view]() {
    Mock::VerifyAndClearExpectations(raw_kbnode_provider_model);
    Mock::VerifyAndClearExpectations(raw_add_kbnode_dialog_view);
  };
}

TEST_F(
    KbNodeAttributeEditPresenterTest,
    should_call_add_kbnode_when_user_clicked_add_kbnode_fake_row) {  // NOLINT
  auto index = index_generator.index();

  // Expectations
  EXPECT_CALL(*kbnode_qmodel, isAddMore(index)).WillOnce(Return(true));

  auto verifier = expectationOnAddKbNode();

  // Exercise system
  userClickedIndex(index);

  // Verify result
  verifier();
}

TEST_F(
    KbNodeAttributeEditPresenterTest,
    should_set_kbnode_to_model_when_user_clicked_a_normal_kbnode_index) {  // NOLINT
  // Setup fixture
  auto index = index_generator.index();
  auto kbnode = xtestutils::genDummyPointer<IKbNode>();

  xtestutils::RandomString kbnode_name;

  // Expectations
  EXPECT_CALL(*kbnode_qmodel, isAddMore(index)).WillOnce(Return(false));
  EXPECT_CALL(*kbnode_qmodel, indexToItem(index)).WillOnce(Return(kbnode));
  EXPECT_CALL(*model, getKbNodeName()).WillOnce(Return(kbnode_name.ustr()));
  {
    InSequence seq;

    EXPECT_CALL(*model, setKbNode(kbnode));
    EXPECT_CALL(*view, setKbNodeName(kbnode_name.qstr()));
    EXPECT_CALL(*view, clearWarningMessages());
  }

  // Exercise system
  userClickedIndex(index);
}

TEST_F(KbNodeAttributeEditPresenterTest,
       should_set_filter_pattern_to_kbnode_provider) {  // NOLINT
  // Setup fixture
  xtestutils::RandomString pattern;

  // Expectations
  EXPECT_CALL(kbnode_provider, setFilterPattern(pattern.ustr()));
  EXPECT_CALL(*view, clearWarningMessages());

  // Exercise system
  filterPatternChanged(pattern.qstr());
}

TEST_F(
    KbNodeAttributeEditPresenterTest,
    should_warn_multiple_match_to_the_user_when_set_kbnode_by_name_when_editing_finished) {  // NOLINT
  // Setup fixture
  xtestutils::RandomString text;

  // Expectations
  EXPECT_CALL(*model, setKbNodeByName(text.ustr()))
      .WillOnce(Return(IKbNodeAttributeModel::kSetKbNodeMultpicMatched));
  EXPECT_CALL(*view, warnMultipleMatch(provider_name.qstr()));

  // Exercise system
  editingFinished(text.qstr());
}

TEST_F(
    KbNodeAttributeEditPresenterTest,
    should_warn_not_match_to_the_user_when_set_kbnode_by_name_when_editing_finished) {  // NOLINT
  // Setup fixture
  xtestutils::RandomString text;

  // Expectations
  EXPECT_CALL(*model, setKbNodeByName(text.ustr()))
      .WillOnce(Return(IKbNodeAttributeModel::kSetKbNodeNotFound));
  EXPECT_CALL(*view, warnNotFound(provider_name.qstr()));

  // Exercise system
  editingFinished(text.qstr());
}

TEST_F(
    KbNodeAttributeEditPresenterTest,
    should_not_warn_the_user_when_successfully_setKbNodeByName_when_editing_finished) {  // NOLINT
  // Setup fixture
  xtestutils::RandomString text;

  // Expectations
  EXPECT_CALL(*model, setKbNodeByName(text.ustr()))
      .WillOnce(Return(IKbNodeAttributeModel::kSetKbNodeSuccess));
  // TODO(lutts): if find by the text got only one entry, do we need to
  // automatically select this only one entry?
  EXPECT_CALL(*view, clearWarningMessages());

  // Exercise system
  editingFinished(text.qstr());
}

TEST_F(
    KbNodeAttributeEditPresenterTest,
    should_call_addKbNode_in_provider_and_notify_qmodel_when_UserClickAddKbNode) {  // NOLINT
  // Expectations
  auto verifier = expectationOnAddKbNode();

  // Exercise system
  userClickAddKbNode();

  // Verify result
  verifier();
}
