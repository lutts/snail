//-*- TestCaseName: KbNodeAttributeTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/kbnode_attribute.h"
#include "core/mock_kbnode_attribute_supplier.h"
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
  std::unique_ptr<KbNodeAttribute> attr;
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
       should_attribute_visitor_visit_KbNodeAttribute) { // NOLINT
  // Setup fixture
  MockAttributeVisitor visitor;

  // Expectations
  EXPECT_CALL(visitor, visit(attr.get()));

  // Exercise system
  GenericAttribute* base_attr = dynamic_cast<GenericAttribute*>(attr.get());
  base_attr->accept(&visitor);
}

////////////////////////////////////////////////

}  // namespace tests
}  // namespace snailcore
