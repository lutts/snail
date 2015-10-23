//-*- TestCaseName: KbNodeAttributeTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/kbnode_attribute.h"
#include "core/mock_kbnode_attribute.h"
#include "snail/mock_kbnode.h"

#include "core/mock_attribute_visitor.h"

namespace snailcore {
namespace tests {

class KbNodeAttributeTest : public ::testing::Test {
 protected:
  KbNodeAttributeTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeAttributeTest() { }
  virtual void SetUp() {
    attr = utils::make_unique<KbNodeAttribute>(&attr_supplier);
  }
  // virtual void TearDown() { }

  void checkEmptyState();
  void setupNonEmptyState();

  // region: objects test subject depends on
  // endregion

  // region: test subject
  std::unique_ptr<IKbNodeAttribute> attr;
  MockKbNodeAttributeSupplier attr_supplier;

  MockKbNode kbnode;
  utils::U8String kbnode_name;
  // endregion

  // region: object depends on test subject
  // endregion
};

void KbNodeAttributeTest::checkEmptyState() {
  ASSERT_TRUE(attr->isEmpty());
  ASSERT_EQ("", attr->valueText());
}

TEST_F(KbNodeAttributeTest,
       check_initial_state) { // NOLINT
  CUSTOM_ASSERT(checkEmptyState());
}

TEST_F(KbNodeAttributeTest,
       should_be_able_to_get_back_the_kbnode_supplier) { // NOLINT
  ASSERT_EQ(&attr_supplier, attr->supplier());
}

TEST_F(KbNodeAttributeTest,
       should_attribute_display_name_be_supplier_name) { // NOLINT
  // Setup fixture
  auto expect_name = xtestutils::genRandomString();

  // Expectations
  EXPECT_CALL(attr_supplier, name()).WillOnce(Return(expect_name));

  // Exercise system
  auto actual_name = attr->displayName();

  // Verify results
  ASSERT_EQ(expect_name, actual_name);
}

void KbNodeAttributeTest::setupNonEmptyState() {
  kbnode_name = xtestutils::genRandomString();
  EXPECT_CALL(kbnode, name()).WillRepeatedly(Return(kbnode_name));

  // Expectations
  EXPECT_CALL(attr_supplier, attributeChanged(attr.get()));

  // Exercise system
  attr->setKbNode(&kbnode);

  // Verify results
  ASSERT_FALSE(attr->isEmpty());
  ASSERT_EQ(kbnode_name, attr->valueText());

  Mock::VerifyAndClearExpectations(&attr_supplier);
}

TEST_F(KbNodeAttributeTest,
       check_state_after_set_an_valid_kbnode) { // NOLINT
  CUSTOM_ASSERT(setupNonEmptyState());
}

TEST_F(KbNodeAttributeTest,
       check_state_after_set_kbnode_to_nullptr) { // NOLINT
  // Setup fixture
  CUSTOM_ASSERT(setupNonEmptyState());

  // Expectations
  EXPECT_CALL(attr_supplier, attributeChanged(attr.get()));

  // Exercise system
  attr->setKbNode(nullptr);

  // Verify results
  CUSTOM_ASSERT(checkEmptyState());
}

TEST_F(KbNodeAttributeTest,
       should_clear_enter_empty_state) { // NOLINT
  // Expectations
  CUSTOM_ASSERT(setupNonEmptyState());

  // Exercise system
  attr->clear();

  // Verify results
  CUSTOM_ASSERT(checkEmptyState());
}

TEST_F(KbNodeAttributeTest,
       should_attribute_visitor_visit_IKbNodeAttribute) { // NOLINT
  // Setup fixture
  MockAttributeVisitor visitor;

  // Expectations
  EXPECT_CALL(visitor, visit(attr.get()));

  // Exercise system
  attr->accept(&visitor);
}

////////////////////////////////////////////////

class KbNodeAttributeSupplierTest : public ::testing::Test {
 protected:
  KbNodeAttributeSupplierTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeAttributeSupplierTest() { }
  virtual void SetUp() {
    expect_max_attrs = std::rand();

    EXPECT_CALL(root_kbnode, name())
        .WillRepeatedly(Return(root_kbnode_name));

    attr_supplier = utils::make_unique<KbNodeAttributeSupplier>(
        &root_kbnode, expect_max_attrs);

    ASSERT_EQ(root_kbnode_name, attr_supplier->name());
    ASSERT_EQ(expect_max_attrs, attr_supplier->max_attrs());
    ASSERT_EQ(&root_kbnode, attr_supplier->getRootKbNode());
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  MockKbNode root_kbnode;
  utils::U8String root_kbnode_name;
  int expect_max_attrs;
  // endregion

  // region: test subject
  std::unique_ptr<IKbNodeAttributeSupplier> attr_supplier;
  // endregion
};

TEST_F(KbNodeAttributeSupplierTest,
       should_createAttribute_create_KbNodeAttribute_instance_with_this_as_the_supplier) { // NOLINT
  // Exercise system
  auto new_attr = attr_supplier->addAttribute();

  // Verify results
  auto actual_attr = dynamic_cast<IKbNodeAttribute*>(new_attr);
  ASSERT_NE(nullptr, actual_attr);

  ASSERT_EQ(attr_supplier.get(), actual_attr->supplier());
}

}  // namespace tests
}  // namespace snailcore
