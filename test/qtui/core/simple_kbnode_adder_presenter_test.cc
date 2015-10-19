//-*- TestCaseName: SimpleKbNodeAdderPresenterTest;-*-
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
#include "snail/mock_simple_kbnode_adder_model.h"
#include "qtui/mock_simple_kbnode_adder_view.h"
#include "src/qtui/core/simple_kbnode_adder_presenter.h"

#include "snail/mock_kbnode_provider.h"
#include "qtui/mock_kbnode_tree_qmodel.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;  // NOLINT
using namespace pfmvp::tests;  // NOLINT

template <typename TestBase>
class SimpleKbNodeAdderPresenterTestBase : public TestBase {
 protected:
  SimpleKbNodeAdderPresenterTestBase() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~SimpleKbNodeAdderPresenterTestBase() { }
  virtual void SetUp() {
    // Setup fixture
    model = std::make_shared<MockSimpleKbNodeAdderModel>();
    view = std::make_shared<MockSimpleKbNodeAdderView>();
    auto kbnode_qmodel_up = utils::make_unique<MockKbNodeTreeQModel>();
    kbnode_qmodel = kbnode_qmodel_up.get();

    // Expectations
    RECORD_USED_MOCK_OBJECTS_SETUP;

    EXPECT_CALL(*model, getKbNodeProvider())
        .WillRepeatedly(Return(&kbnode_provider));

    {
      InSequence seq;

      // use current filter pattern as the default node name
      {
        InSequence seq;
        xtestutils::RandomString filter_pattern;
        bool init_validate_result = xtestutils::randomBool();

        // tell view the provider name
        xtestutils::RandomString provider_name;
        EXPECT_CALL(*model, name()).WillOnce(Return(provider_name.ustr()));
        EXPECT_CALL(*view, setProviderName(provider_name.qstr()));

        // use filter pattern as the default new kbnode name
        R_EXPECT_CALL(*model, getFilterPattern())
            .WillOnce(Return(filter_pattern.ustr()));
        R_EXPECT_CALL(*model, setNewKbNodeName(filter_pattern.ustr()));

        R_EXPECT_CALL(*view, setNewKbNodeName(filter_pattern.qstr()));
        R_EXPECT_CALL(*model, isNewKbNodeNameValid())
            .WillOnce(Return(init_validate_result));
        R_EXPECT_CALL(*view, setNameValidateResult(init_validate_result));

        // then clear filter pattern (aka. exit filter mode)
        R_EXPECT_CALL(*model, setFilterPattern(""));

        // init qmodel
        R_EXPECT_CALL(*kbnode_qmodel, setKbNodeProvider(&kbnode_provider));
        R_EXPECT_CALL(*view, setKbNodeTreeQModel(kbnode_qmodel));
      }

      // highlight the current parent node
      auto index = index_generator.index();
      auto kbnode = xtestutils::genDummyPointer<IKbNode>();
      R_EXPECT_CALL(*model, getNewKbNodeParent())
          .WillOnce(Return(kbnode));
      R_EXPECT_CALL(*kbnode_qmodel, kbNodeToIndex(kbnode))
          .WillOnce(Return(index));
      R_EXPECT_CALL(*view, selectIndex(index));
    }

    R_EXPECT_CALL(*view, whenUserSelectIndex(_, _))
        .WillOnce(SaveArg<0>(&userSelectIndex));

    R_EXPECT_CALL(*view, whenNewKbNodeNameChanged(_, _))
        .WillOnce(SaveArg<0>(&newKbNodeNameChanged));

    R_EXPECT_CALL(*view, whenUserToggleCategoryCheckbox(_, _))
        .WillOnce(SaveArg<0>(&userToggleCategoryCheckbox));

    R_EXPECT_CALL(*view, whenUserClickAddButton(_, _))
        .WillOnce(SaveArg<0>(&userClickAddButton));

    // Excercise system
    presenter = std::make_shared<SimpleKbNodeAdderPresenter>(
        model, view, std::move(kbnode_qmodel_up));
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  std::shared_ptr<MockSimpleKbNodeAdderModel> model;
  std::shared_ptr<MockSimpleKbNodeAdderView> view;

  MockKbNodeProvider kbnode_provider;
  MockKbNodeTreeQModel* kbnode_qmodel;
  QModelIndexGenerator index_generator;

  MockPfTriadManager triad_manager;
  // endregion

  // region: test subject
  std::shared_ptr<SimpleKbNodeAdderPresenter> presenter;
  // endregion

  // region: object depends on test subject
  SlotCatcher<ISimpleKbNodeAdderView::UserSelectIndexSlotType> userSelectIndex;

  using NewKbNodeNameChangedSlotType =
      ISimpleKbNodeAdderView::NewKbNodeNameChangedSlotType;
  SlotCatcher<NewKbNodeNameChangedSlotType> newKbNodeNameChanged;

  using UserClickAddButtonSlotType =
      ISimpleKbNodeAdderView::UserClickAddButtonSlotType;
  SlotCatcher<UserClickAddButtonSlotType> userClickAddButton;

  using UserToggleCategoryCheckboxSlotType =
      ISimpleKbNodeAdderView::UserToggleCategoryCheckboxSlotType;
  SlotCatcher<UserToggleCategoryCheckboxSlotType> userToggleCategoryCheckbox;
  // endregion
};

class SimpleKbNodeAdderPresenterTest
    : public SimpleKbNodeAdderPresenterTestBase<::testing::Test> { };

TEST_F(SimpleKbNodeAdderPresenterTest,
       should_set_current_select_kbnode_as_new_kbnode_parent) { // NOLINT
  auto index = index_generator.index();
  auto kbnode = xtestutils::genDummyPointer<IKbNode>();
  // Expectations
  EXPECT_CALL(*kbnode_qmodel, indexToKbNode(index))
      .WillOnce(Return(kbnode));

  EXPECT_CALL(*model, setNewKbNodeParent(kbnode));

  // Exercise system
  userSelectIndex(index);
}

class SimpleKbNodeAdderPresenterTest_BoolParam
    : public SimpleKbNodeAdderPresenterTestBase<::testing::TestWithParam<bool>> {};

INSTANTIATE_TEST_CASE_P(BoolParam,
                        SimpleKbNodeAdderPresenterTest_BoolParam,
                        ::testing::Bool());

TEST_P(SimpleKbNodeAdderPresenterTest_BoolParam,
       should_set_user_entered_new_kbnode_name_to_model_and_update_view_with_validate_status) { // NOLINT
  // Setup fixture
  xtestutils::RandomString new_name;
  bool validate_result = GetParam();

  // Expectations
  {
    InSequence seq;

    EXPECT_CALL(*model, setNewKbNodeName(new_name.ustr()));
    EXPECT_CALL(*model, isNewKbNodeNameValid())
        .WillOnce(Return(validate_result));
    EXPECT_CALL(*view, setNameValidateResult(validate_result));
  }

  // Exercise system
  newKbNodeNameChanged(new_name.qstr());
}

TEST_P(SimpleKbNodeAdderPresenterTest_BoolParam,
       should_set_isCategory_status_to_model_when_user_toggle_category_checkbox) { // NOLINT
  bool checked = GetParam();
  // Expectations
  EXPECT_CALL(*model, setIsCategory(checked));

  // Exercise system
  userToggleCategoryCheckbox(checked);
}

TEST_F(SimpleKbNodeAdderPresenterTest,
       should_call_model_addKbNode_when_user_click_Add_button) { // NOLINT
  // Setup fixture

  // Expectations
  EXPECT_CALL(*model, addKbNode());

  // Exercise system
  userClickAddButton();
}