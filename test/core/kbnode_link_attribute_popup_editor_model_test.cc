//-*- TestCaseName: KbNodeLinkAttributePopupEditorModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/kbnode_link_attribute_popup_editor_model.h"
#include "core/mock_kbnode_link_attribute.h"
#include "snail/mock_attribute.h"
#include "snail/mock_attribute_model.h"
#include "core/mock_attribute_model_factory.h"
#include "snail/mock_tree_item_provider.h"
// #include "core/mock_link_type.h"

namespace snailcore {
namespace tests {

class KbNodeLinkAttributePopupEditorModelTest : public ::testing::Test {
 protected:
  KbNodeLinkAttributePopupEditorModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeLinkAttributePopupEditorModelTest() { }
  void SetUp() override {
    model = utils::make_unique<KbNodeLinkAttributePopupEditorModel>(
        &link_attr, &attr_model_factory);

    EXPECT_CALL(link_attr, supplier())
        .WillRepeatedly(Return(&link_attr_supplier));
    EXPECT_CALL(link_attr_supplier, getLinkTypeProvider())
        .WillRepeatedly(Return(&link_type_provider));
  }
  // void TearDown() override { }

  // region: objects test subject depends on
  MockKbNodeLinkAttributeSupplier link_attr_supplier;
  MockKbNodeLinkAttribute link_attr;

  MockTreeItemProvider link_type_provider;

  MockAttribute value_attr;
  MockAttribute value_attr_copy;

  MockAttributeModelFactory attr_model_factory;
  // endregion

  // region: test subject
  std::unique_ptr<KbNodeLinkAttributePopupEditorModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(KbNodeLinkAttributePopupEditorModelTest, should_construct_properly) { // NOLINT
  // See SetUp()
  SUCCEED();
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_create_value_attr_model) { // NOLINT
  // Setup fixture
  auto expect_value_model = std::make_shared<MockAttributeModel>();

  // Expectations
  EXPECT_CALL(link_attr, valueAttr())
      .WillOnce(Return(&value_attr));
  EXPECT_CALL(value_attr, clone()).WillOnce(Return(&value_attr_copy));
  EXPECT_CALL(attr_model_factory, createAttributeModel(&value_attr_copy))
      .WillOnce(Return(expect_value_model));

  // Exercise system
  auto actual_value_model = model->createValueKbNodeAttrModel();

  // Verify results
  ASSERT_EQ(expect_value_model, actual_value_model);
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_return_the_link_provider) { // NOLINT
  // Exercise system
  auto actual_link_provider = model->getLinkTypeProvider();

  // Verify results
  ASSERT_EQ(&link_type_provider, actual_link_provider);
}

#if 0
TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_return_current_link_type) { // NOLINT
  // Setup fixture

  // Expectations
  EXPECT_CALL(*link_attr, linkType())
      .WillOnce(Return(link_type));
  EXPECT_CALL(link_type, clone()).WillOnce(Return(&link_type_copy));

  // Exercise system
  auto actual_link_type = model->getCurrentLinkType();

  // Verify results
  ASSERT_EQ(&link_type_copy, actual_link_type);

  // get again will not make another copy
  auto actual_link_type_again = model->getCurrentLinkType();
  ASSERT_EQ(&link_type_copy, actual_link_type);
}
#endif


}  // namespace tests
}  // namespace snailcore
