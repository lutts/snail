//-*- TestCaseName: KbNodeLinkAttributeInlineEditorModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/kbnode_link_attribute_inline_editor_model.h"
#include "snail/mock_kbnode_link_attribute_popup_editor_model.h"
#include "core/mock_kbnode_link_attribute_popup_editor_model_factory.h"
#include "core/mock_kbnode_link_attribute.h"

namespace snailcore {
namespace tests {

class KbNodeLinkAttributeInlineEditorModelTest : public ::testing::Test {
 protected:
  KbNodeLinkAttributeInlineEditorModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeLinkAttributeInlineEditorModelTest() { }
  void SetUp() override {
    model = utils::make_unique<KbNodeLinkAttributeInlineEditorModel>(
        &kbnode_link_attr, &popup_editor_model_factory);
  }
  // void TearDown() override { }

  // region: objects test subject depends on
  MockKbNodeLinkAttribute kbnode_link_attr;
  MockKbNodeLinkAttributePopupEditorModelFactory popup_editor_model_factory;
  // endregion

  // region: test subject
  std::unique_ptr<KbNodeLinkAttributeInlineEditorModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(KbNodeLinkAttributeInlineEditorModelTest,
       should_valueText_be_attr_value_text) {  // NOLINT
  // Setup fixture
  auto expect_text = xtestutils::genRandomString();
  EXPECT_CALL(kbnode_link_attr, valueText()).WillOnce(Return(expect_text));

  // Exercise system
  auto actual_text = model->valueText();

  // Verify results
  ASSERT_EQ(expect_text, actual_text);
}

TEST_F(KbNodeLinkAttributeInlineEditorModelTest,
       should_be_able_to_create_popup_editor_model) {  // NOLINT
  // Setup fixture
  auto expect_model =
      std::make_shared<MockKbNodeLinkAttributePopupEditorModel>();

  // Expectations
  EXPECT_CALL(popup_editor_model_factory,
              createPopupEditorModel(&kbnode_link_attr))
      .WillOnce(Return(expect_model));

  // Exercise system
  auto actual_model = model->createPopupEditorModel();

  // Verify results
  ASSERT_EQ(expect_model, actual_model);
}

}  // namespace tests
}  // namespace snailcore
