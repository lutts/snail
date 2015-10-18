//-*- TestCaseName: GenericAttributeTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "core/generic_attribute.h"
#include "snail/mock_attribute_supplier.h"

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

  MOCK_METHOD1(accept, void(IAttributeVisitor* visitor));

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

}  // namespace tests
}  // namespace snailcore
