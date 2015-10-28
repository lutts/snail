//-*- TestCaseName: KbNodeLinkAttributePopupEditorModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/kbnode_link_attribute_popup_editor_model.h"

#include "src/core/kbnode_attribute.h"
#include "core/mock_kbnode_link_attribute.h"
#include "snail/mock_attribute_supplier.h"
#include "snail/mock_attribute_model.h"
#include "core/mock_attribute_model_factory.h"
#include "core/mock_attribute_set_model_factory.h"
#include "snail/mock_attribute_set_model.h"
#include "snail/mock_tree_item_provider.h"
#include "core/mock_kbnode.h"
#include "src/core/link_type.h"

namespace snailcore {

bool operator==(const KbNodeAttribute& lhs, const KbNodeAttribute& rhs) {
  return (lhs.supplier() == rhs.supplier()) &&
      (lhs.valueText() == rhs.valueText());
}

bool operator==(const LinkType& lhs, const LinkType& rhs) {
  return lhs.name() == rhs.name();
}

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
        &link_attr, &attr_model_factory, &attr_set_model_factory);
  }
  // void TearDown() override { }

  void checkedGetCurrentValueAttr(
      KbNodeAttribute** value_attr_local_copy = nullptr);

  // region: objects test subject depends on
  MockKbNodeLinkAttributeSupplier link_attr_supplier;
  MockKbNodeLinkAttribute link_attr;

  MockAttributeModelFactory attr_model_factory;
  MockAttributeSetModelFactory attr_set_model_factory;
  // endregion

  // region: test subject
  std::unique_ptr<KbNodeLinkAttributePopupEditorModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

void KbNodeLinkAttributePopupEditorModelTest::checkedGetCurrentValueAttr(
    KbNodeAttribute** value_attr_local_copy) {
  // Setup fixture
  MockKbNode kbnode;
  EXPECT_CALL(kbnode, name())
      .WillRepeatedly(Return(xtestutils::genRandomString()));
  KbNodeAttribute value_attr;
  value_attr.setKbNode(&kbnode);
  EXPECT_CALL(link_attr, valueAttr())
      .WillOnce(Return(&value_attr));

  // Exercise system
  auto value_attr_copy = model->getCurrentValueAttr();

  // Verify results
  ASSERT_NE(nullptr, value_attr_copy);
  CUSTOM_ASSERT(assertObjectCopyEq(*value_attr_copy, value_attr));

  if (value_attr_local_copy)
    *value_attr_local_copy = value_attr_copy;
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_get_current_value_attr) { // NOLINT
  CUSTOM_ASSERT(checkedGetCurrentValueAttr());
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_create_value_attribute_model) { // NOLINT
  // Setup fixture
  KbNodeAttribute* value_attr_copy = nullptr;
  CUSTOM_ASSERT(checkedGetCurrentValueAttr(&value_attr_copy));

  auto value_attr_model = std::make_shared<MockAttributeModel>();
  EXPECT_CALL(attr_model_factory,
              createAttributeModel(value_attr_copy))
      .WillOnce(Return(value_attr_model));

  // Exercise system
  auto actual_attr_model = model->createValueAttrModel();

  // Verify results
  ASSERT_EQ(value_attr_model, actual_attr_model);
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_return_link_type_item_provider) { // NOLINT
  // Setup fixture
  EXPECT_CALL(link_attr, supplier()).WillOnce(Return(&link_attr_supplier));
  auto expect_item_provider =
      xtestutils::genDummyPointer<ITreeItemProvider>();
  EXPECT_CALL(link_attr_supplier, getLinkTypeItemProvider())
      .WillOnce(Return(expect_item_provider));

  // Exercise system
  auto actual_item_provider = model->getLinkTypeItemProvider();

  // Verify results
  ASSERT_EQ(expect_item_provider, actual_item_provider);
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_get_current_link_type) { // NOLINT
  // Setup fixture
  LinkType link_type { xtestutils::genRandomString(), false };
  EXPECT_CALL(link_attr, linkType()).WillOnce(Return(&link_type));

  // Exercise system
  auto link_type_item = model->getCurrentLinkType();

  // Verify results
  auto actual_link_type = dynamic_cast<LinkType*>(link_type_item);
  ASSERT_NE(nullptr, actual_link_type);
  CUSTOM_ASSERT(assertObjectCopyEq(link_type, *actual_link_type));

  auto link_type_item_again = model->getCurrentLinkType();
  ASSERT_EQ(link_type_item, link_type_item_again)
      << "should got the same link type item when get again";
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_create_attribute_set_model_for_current_link_type) { // NOLINT
  // Setup fixture
  LinkType link_type { xtestutils::genRandomString(), false };
  EXPECT_CALL(link_attr, linkType()).WillOnce(Return(&link_type));

  std::vector<std::unique_ptr<IAttributeSupplier>> expect_attr_suppliers_orig;
  std::vector<IAttributeSupplier*> expect_attr_suppliers;

  const int TEST_SUPPLIER_COUNT = 3;

  for (int i = 0; i < TEST_SUPPLIER_COUNT; ++i) {
    auto supplier = utils::make_unique<MockAttributeSupplier>();

    IAttributeSupplier* supplier_clone = new MockAttributeSupplier();
    EXPECT_CALL(*supplier, clone()).WillOnce(Return(supplier_clone));

    expect_attr_suppliers_orig.push_back(std::move(supplier));
    expect_attr_suppliers.push_back(supplier_clone);
  }

  link_type.setAttributeSuppliers(std::move(expect_attr_suppliers_orig));

  std::vector<IAttributeSupplier*> actual_attr_suppliers;
  auto expect_attr_set_model = std::make_shared<MockAttributeSetModel>();

  EXPECT_CALL(attr_set_model_factory, createAttributeSetModel(_))
      .WillOnce(DoAll(
          SaveArg<0>(&actual_attr_suppliers),
          Return(expect_attr_set_model)));

  // Exercise system
  auto actual_attr_set_model = model->getCurrentLinkAttrSetModel();

  // Verify results
  ASSERT_EQ(expect_attr_set_model, actual_attr_set_model);
  ASSERT_EQ(expect_attr_suppliers, actual_attr_suppliers);
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_make_a_copy_when_user_set_link_type) { // NOLINT
  // Setup fixture
  LinkType new_link_type { xtestutils::genRandomString(), false };

  // Exercise system
  model->setLinkType(&new_link_type);

  // Verify results
  auto curr_link_type_item = model->getCurrentLinkType();
  auto curr_link_type = dynamic_cast<LinkType*>(curr_link_type_item);
  ASSERT_NE(nullptr, curr_link_type);
  CUSTOM_ASSERT(assertObjectCopyEq(new_link_type, *curr_link_type));
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_sync_local_copy_to_link_attr_when_edit_finished) { // NOLINT
  // Setup fixture
  MockKbNode kbnode;
  EXPECT_CALL(kbnode, name())
      .WillRepeatedly(Return(xtestutils::genRandomString()));
  KbNodeAttribute value_attr;
  value_attr.setKbNode(&kbnode);
  EXPECT_CALL(link_attr, valueAttr()).WillRepeatedly(Return(&value_attr));

  auto value_attr_local_copy = model->getCurrentValueAttr();

  LinkType link_type { xtestutils::genRandomString(), false };
  EXPECT_CALL(link_attr, linkType()).WillRepeatedly(Return(&link_type));
  auto link_type_item = model->getCurrentLinkType();
  auto link_type_local_copy = dynamic_cast<LinkType*>(link_type_item);
  ASSERT_NE(nullptr, link_type_local_copy);

  // simulate user modify the local copies
  MockKbNode new_kbnode;
  EXPECT_CALL(new_kbnode, name())
      .WillRepeatedly(Return(xtestutils::genRandomString()));
  value_attr_local_copy->setKbNode(&new_kbnode);

  std::vector<std::unique_ptr<IAttributeSupplier>> dummy_attr_suppliers;
  std::vector<IAttributeSupplier*> expect_attr_suppliers;
  const int TEST_SUPPLIER_COUNT = 3;
  for (int i = 0; i < TEST_SUPPLIER_COUNT; ++i) {
    auto supplier = utils::make_unique<MockAttributeSupplier>();

    IAttributeSupplier* supplier_clone = new MockAttributeSupplier();
    EXPECT_CALL(*supplier, clone()).WillOnce(Return(supplier_clone));

    dummy_attr_suppliers.push_back(std::move(supplier));
    expect_attr_suppliers.push_back(supplier_clone);
  }

  link_type_local_copy->setAttributeSuppliers(std::move(dummy_attr_suppliers));

  // Exercise system
  model->editFinished();

  // Verify results
  CUSTOM_ASSERT(assertObjectCopyEq(*value_attr_local_copy, value_attr));
  CUSTOM_ASSERT(assertObjectCopyEq(*link_type_local_copy, link_type));
  ASSERT_EQ(expect_attr_suppliers, link_type.attributeSuppliers());
}

}  // namespace tests
}  // namespace snailcore
