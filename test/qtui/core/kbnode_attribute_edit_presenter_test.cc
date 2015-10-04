//-*- TestCaseName: KbNodeAttributeEditPresenterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "pfmvp/mock_pf_triad_manager.h"
#include "test/testutils/qmodelindex_generator.h"
#include "test/testutils/random_string.h"

// triad headers
#include "snail/mock_kbnode_attribute_model.h"
#include "snail/mock_kbnode_provider.h"
#include "qtui/mock_kbnode_attribute_edit_view.h"
#include "qtui/mock_kbnode_tree_qmodel.h"
#include "src/qtui/core/kbnode_attribute_edit_presenter.h"
#include "snail/mock_kbnode.h"

#include "snail/mock_kbnode_provider_model.h"
#include "qtui/mock_kbnode_provider_view.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;  // NOLINT
using namespace pfmvp::tests;  // NOLINT

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
    auto kbnode_qmodel_up = utils::make_unique<MockKbNodeTreeQModel>();
    kbnode_qmodel = kbnode_qmodel_up.get();

    // Expectations
    RECORD_USED_MOCK_OBJECTS_SETUP;

    xtestutils::RandomString kbnode_name;

    R_EXPECT_CALL(*model, getKbNodeName())
        .WillOnce(Return(kbnode_name.ustr()));
    R_EXPECT_CALL(*view, setKbNodeName(kbnode_name.qstr()));

    EXPECT_CALL(*model, getKbNodeProvider())
        .WillRepeatedly(Return(&kbnode_provider));
    {
      InSequence seq;

      R_EXPECT_CALL(*kbnode_qmodel, setKbNodeProvider(&kbnode_provider));
      R_EXPECT_CALL(*view, setKbNodeTreeQModel(kbnode_qmodel));
    }

    R_EXPECT_CALL(*view, whenUserClickedIndex(_, _))
        .WillOnce(SaveArg<0>(&userClickedIndex));

    R_EXPECT_CALL(*view, whenFilterPatternChanged(_, _))
        .WillOnce(SaveArg<0>(&filterPatternChanged));

    R_EXPECT_CALL(*view, whenEditingFinished(_, _))
        .WillOnce(SaveArg<0>(&editingFinished));

    R_EXPECT_CALL(*view, whenUserClickAddKbNode(_, _))
        .WillOnce(SaveArg<0>(&userClickAddKbNode));

    R_EXPECT_CALL(kbnode_provider, whenBeginFilter(_, _))
        .WillOnce(SaveArg<0>(&providerBeginFilter));

    R_EXPECT_CALL(kbnode_provider, whenFinishFilter(_, _))
        .WillOnce(SaveArg<0>(&providerFinishFilter));

    // Excercise system
    presenter =
        std::make_shared<KbNodeAttributeEditPresenter>(
            model, view, std::move(kbnode_qmodel_up));
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;

    EXPECT_CALL(*model, getKbNodeProvider())
        .WillRepeatedly(Return(&kbnode_provider));
  }
  // virtual void TearDown() { }

  void expectationOnAddKbNode();
  void shouldNotifyKbNodeTreeQModelWhenKbNodeAdded();

  // region: objects test subject depends on
  std::shared_ptr<MockKbNodeAttributeModel> model;
  std::shared_ptr<MockKbNodeAttributeEditView> view;

  MockKbNodeProvider kbnode_provider;
  MockKbNodeTreeQModel* kbnode_qmodel;

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

  using BeginFilterSlotType =
      IKbNodeProvider::BeginFilterSlotType;
  SlotCatcher<BeginFilterSlotType> providerBeginFilter;

  using FinishFilterSlotType =
      IKbNodeProvider::FinishFilterSlotType;
  SlotCatcher<FinishFilterSlotType> providerFinishFilter;

  using KbNodeAddedSlotType =
      IKbNodeProviderModel::KbNodeAddedSlotType;
  SlotCatcher<KbNodeAddedSlotType> kbNodeAdded;
  // endregion
};

void KbNodeAttributeEditPresenterTest::expectationOnAddKbNode() {
  auto kbnode_provider_model = std::make_shared<MockKbNodeProviderModel>();
  std::shared_ptr<IPfModel> kbnode_provider_pfmodel = kbnode_provider_model;
  auto add_kbnode_dialog_view = std::make_shared<MockKbNodeProviderView>();

  EXPECT_CALL(*model, createKbNodeProviderModel())
      .WillOnce(Return(kbnode_provider_model));
  EXPECT_CALL(triad_manager, createViewFor(kbnode_provider_pfmodel, _, _, _))
      .WillOnce(Return(add_kbnode_dialog_view));
  EXPECT_CALL(*add_kbnode_dialog_view, showView(true));

  EXPECT_CALL(*kbnode_provider_model, whenKbNodeAdded(_, _))
      .WillOnce(SaveArg<0>(&kbNodeAdded));
}

void KbNodeAttributeEditPresenterTest::shouldNotifyKbNodeTreeQModelWhenKbNodeAdded() { // NOLINT
  // Setup fixture
  auto parent_kbnode = xtestutils::genDummyPointer<IKbNode>();
  auto new_kbnode = xtestutils::genDummyPointer<IKbNode>();

  // Expectations
  EXPECT_CALL(*kbnode_qmodel, kbNodeAdded(new_kbnode, parent_kbnode));

  // Excercise system
  kbNodeAdded(new_kbnode, parent_kbnode);
}

TEST_F(KbNodeAttributeEditPresenterTest,
       should_call_add_kbnode_when_user_clicked_add_kbnode_fake_row) { // NOLINT
  auto index = index_generator.index();

  // Expectations
  EXPECT_CALL(*kbnode_qmodel, isAddKbNode(index))
      .WillOnce(Return(true));

  expectationOnAddKbNode();

  // Exercise system
  userClickedIndex(index);

  // Verify result
  shouldNotifyKbNodeTreeQModelWhenKbNodeAdded();
}

TEST_F(KbNodeAttributeEditPresenterTest,
       should_set_kbnode_to_model_when_user_clicked_a_normal_kbnode_index) { // NOLINT
  // Setup fixture
  auto index = index_generator.index();
  auto kbnode = xtestutils::genDummyPointer<IKbNode>();

  xtestutils::RandomString kbnode_name;

  // Expectations
  EXPECT_CALL(*kbnode_qmodel, isAddKbNode(index))
      .WillOnce(Return(false));
  EXPECT_CALL(*kbnode_qmodel, indexToKbNode(index))
      .WillOnce(Return(kbnode));
  EXPECT_CALL(*model, getKbNodeName())
      .WillOnce(Return(kbnode_name.ustr()));
  {
    InSequence seq;

    EXPECT_CALL(*model, setKbNode(kbnode));
    EXPECT_CALL(*view, setKbNodeName(kbnode_name.qstr()));
  }

  // Exercise system
  userClickedIndex(index);
}

TEST_F(KbNodeAttributeEditPresenterTest,
       should_set_filter_pattern_to_kbnode_provider) { // NOLINT
  // Setup fixture
  xtestutils::RandomString pattern;

  // Expectations
  EXPECT_CALL(kbnode_provider, setFilterPattern(pattern.ustr()));

  // Exercise system
  filterPatternChanged(pattern.qstr());
}

TEST_F(KbNodeAttributeEditPresenterTest,
       should_beginResetQModel_when_kbnode_provider_begin_filter) { // NOLINT
  // Expectations
  EXPECT_CALL(*kbnode_qmodel, beginResetQModel());

  // Exercise system
  providerBeginFilter();
}

TEST_F(KbNodeAttributeEditPresenterTest,
       should_endResetQModel_when_kbnode_provider_finish_filter) { // NOLINT
  // Expectations
  EXPECT_CALL(*kbnode_qmodel, endResetQModel());

  // Exercise system
  providerFinishFilter();
}

TEST_F(KbNodeAttributeEditPresenterTest,
       should_warn_multiple_match_to_the_user_when_set_kbnode_by_name_when_editing_finished) { // NOLINT
  // Setup fixture
  xtestutils::RandomString text;

  // Expectations
  EXPECT_CALL(*model, setKbNodeByName(text.ustr()))
      .WillOnce(Return(IKbNodeAttributeModel::kSetKbNodeMultpicMatched));
  EXPECT_CALL(*view, warnMultipleMatch());

  // Exercise system
  editingFinished(text.qstr());
}

TEST_F(KbNodeAttributeEditPresenterTest,
       should_warn_not_match_to_the_user_when_set_kbnode_by_name_when_editing_finished) { // NOLINT
  // Setup fixture
  xtestutils::RandomString text;

  // Expectations
  EXPECT_CALL(*model, setKbNodeByName(text.ustr()))
      .WillOnce(Return(IKbNodeAttributeModel::kSetKbNodeNotFound));
  EXPECT_CALL(*view, warnNotFound());

  // Exercise system
  editingFinished(text.qstr());
}

TEST_F(KbNodeAttributeEditPresenterTest,
       should_not_warn_the_user_when_successfully_setKbNodeByName_when_editing_finished) { // NOLINT
  // Setup fixture
  xtestutils::RandomString text;

  // Expectations
  EXPECT_CALL(*model, setKbNodeByName(text.ustr()))
      .WillOnce(Return(IKbNodeAttributeModel::kSetKbNodeSuccess));
  // TODO(lutts): if find by the text got only one entry, do we need to
  // automatically select this only one entry?
  EXPECT_CALL(*view, warnMultipleMatch()).Times(0);

  // Exercise system
  editingFinished(text.qstr());
}

TEST_F(KbNodeAttributeEditPresenterTest,
       should_call_addKbNode_in_provider_and_notify_qmodel_when_UserClickAddKbNode) { // NOLINT
  // Expectations
  expectationOnAddKbNode();

  // Exercise system
  userClickAddKbNode();

  // Verify result
  shouldNotifyKbNodeTreeQModelWhenKbNodeAdded();
}
