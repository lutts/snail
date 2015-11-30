//-*- TestCaseName: KbNodeLinkAttributeInlineEditorPresenterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "pfmvp/mock_pf_triad_manager.h"
#include "test/testutils/random_string.h"

// triad headers
#include "snail/mock_kbnode_link_attribute_inline_editor_model.h"
#include "qtui/ui/mock_kbnode_link_attribute_inline_editor_view.h"
#include "src/qtui/core/kbnode_link_attribute_inline_editor_presenter.h"

// triad headers
#include "snail/mock_kbnode_link_attribute_popup_editor_model.h"
#include "qtui/ui/mock_kbnode_link_attribute_popup_editor_view.h"

using namespace snailcore;         // NOLINT
using namespace snailcore::tests;  // NOLINT
using namespace pfmvp;             // NOLINT
using namespace pfmvp::tests;      // NOLINT

class KbNodeLinkAttributeInlineEditorPresenterTest;

class UpdateValueTestFixture : public TestFixture {
 public:
  UpdateValueTestFixture(
      const utils::U8String& name,
      KbNodeLinkAttributeInlineEditorPresenterTest* test_case);
};

class KbNodeLinkAttributeInlineEditorPresenterTest : public ::testing::Test {
 public:
  KbNodeLinkAttributeInlineEditorPresenterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeLinkAttributeInlineEditorPresenterTest() { }
  virtual void SetUp() {
    // Setup fixture
    model = std::make_shared<MockKbNodeLinkAttributeInlineEditorModel>();
    view = std::make_shared<MockKbNodeLinkAttributeInlineEditorView>();

    // Expectations
    RECORD_USED_MOCK_OBJECTS_SETUP;

    FixtureHelper(UpdateValueTestFixture, fixture);

    R_EXPECT_CALL(*view, whenUserClickShowPopupEditor(_, _))
        .WillOnce(DoAll(SaveArg<0>(&userClickShowPopupEditor),
                        Return(SignalConnection())));

    // Excercise system
    presenter =
        std::make_shared<KbNodeLinkAttributeInlineEditorPresenter>(model, view);
    presenter->set_triad_manager(&triad_manager);
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  std::shared_ptr<MockKbNodeLinkAttributeInlineEditorModel> model;
  std::shared_ptr<MockKbNodeLinkAttributeInlineEditorView> view;

  MockPfTriadManager triad_manager;
  // endregion

  // region: test subject
  std::shared_ptr<KbNodeLinkAttributeInlineEditorPresenter> presenter;
  // endregion

  // region: object depends on test subject
  using UserClickShowPopupEditorSlotType =
      IKbNodeLinkAttributeInlineEditorView::UserClickShowPopupEditorSlotType;
  SlotCatcher<UserClickShowPopupEditorSlotType> userClickShowPopupEditor;
  // endregion
};

UpdateValueTestFixture::UpdateValueTestFixture(
    const utils::U8String& name,
    KbNodeLinkAttributeInlineEditorPresenterTest* test_case)
    : TestFixture(name) {
  xtestutils::RandomString attr_value_text;
  R_EXPECT_CALL(*(test_case->model), valueText())
      .WillOnce(Return(attr_value_text.ustr()));
  R_EXPECT_CALL(*(test_case->view), setValueText(attr_value_text.qstr()));
}

TEST_F(KbNodeLinkAttributeInlineEditorPresenterTest,
       should_be_able_to_show_popup_editor_view) {  // NOLINT
  // Setup fixture
  auto popup_editor_model =
      std::make_shared<MockKbNodeLinkAttributePopupEditorModel>();
  auto popup_editor_view =
      std::make_shared<MockKbNodeLinkAttributePopupEditorView>();

  // Expectations
  EXPECT_CALL(*model, createPopupEditorModel())
      .WillOnce(Return(popup_editor_model));

  using VT = MockKbNodeLinkAttributePopupEditorView;
  triad_manager.expectationsOnShowModalDialog<VT>(popup_editor_model,
                                                  popup_editor_view);

  FixtureHelper(UpdateValueTestFixture, fixture);

  // Exercise system
  userClickShowPopupEditor();
}
