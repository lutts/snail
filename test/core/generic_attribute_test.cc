//-*- TestCaseName: GenericAttributeTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/generic_attribute.h"
#include "snail/mock_attribute_supplier.h"

/*
1. provide protected dataChanged() to be called by the sub class, which in turn call suppliers attributeChanged();
2. clear will call dataChanged() automatically, and will call do_clear() of sub class;
3. isEmpty() is not implemented;
4. valueText() default to return empty string;
5. displayName() default to return the supplier's name;
*/

namespace snailcore {
namespace tests {

class MockGenericAttribute : public GenericAttribute {
 public:
  explicit MockGenericAttribute(IAttributeSupplier* supplier)
      : GenericAttribute(supplier) { }
  virtual ~MockGenericAttribute() { }

  MOCK_CONST_METHOD0(valueText, utils::U8String());
  MOCK_CONST_METHOD0(isEmpty, bool());
  MOCK_METHOD0(clear, void());

  void trigger_dataChanged() {
    dataChanged();
  }

  IAttributeSupplier* getSupplier() {
    return supplier();
  }

 private:
  SNAIL_DISABLE_COPY(MockGenericAttribute)
};

class GenericAttributeTest : public ::testing::Test {
 protected:
  GenericAttributeTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~GenericAttributeTest() { }
  virtual void SetUp() {
    attr = utils::make_unique<MockGenericAttribute>(&attr_supplier);
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  MockAttributeSupplier attr_supplier;
  // endregion

  // region: test subject
  std::unique_ptr<MockGenericAttribute> attr;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(GenericAttributeTest, should_construct_properly) { // NOLINT
  // See SetUp()
  SUCCEED();
}

TEST_F(GenericAttributeTest,
       should_displayName_default_to_supplier_name) { // NOLINT
  // Setup fixture
  auto supplier_name = xtestutils::genRandomString();
  EXPECT_CALL(attr_supplier, name())
      .WillOnce(Return(supplier_name));

  // Verify results
  ASSERT_EQ(supplier_name, attr->displayName());
}

TEST_F(GenericAttributeTest,
       should_dataChanged_call_supplier_attributeChanged) { // NOLINT
  // Expectations
  EXPECT_CALL(attr_supplier, attributeChanged(attr.get()));

  // Exercise system
  attr->trigger_dataChanged();
}

TEST_F(GenericAttributeTest,
       should_supplier_be_the_supplier_passed_to_constructor) { // NOLINT
  ASSERT_EQ(&attr_supplier, attr->getSupplier());
}

}  // namespace tests
}  // namespace snailcore
