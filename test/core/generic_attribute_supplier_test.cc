//-*- TestCaseName: GenericAttributeSupplierTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "core/generic_attribute_supplier.h"
#include "snail/mock_attribute.h"

namespace snailcore {
namespace tests {

class MockGenericAttributeSupplier : public GenericAttributeSupplier {
 public:
  MockGenericAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }

  MOCK_METHOD0(createAttribute, IAttribute*());
};

class GenericAttributeSupplierTest : public ::testing::Test {
 protected:
  GenericAttributeSupplierTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~GenericAttributeSupplierTest() { }
  virtual void SetUp() {
    expect_name = xtestutils::genRandomString();
    expect_max_attrs = std::rand();

    supplier = utils::make_unique<MockGenericAttributeSupplier>(
        expect_name, expect_max_attrs);
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  utils::U8String expect_name;
  int expect_max_attrs;
  // endregion

  // region: test subject
  std::unique_ptr<MockGenericAttributeSupplier> supplier;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(GenericAttributeSupplierTest, should_construct_properly) { // NOLINT
  // See SetUp()
  SUCCEED();
}

TEST_F(GenericAttributeSupplierTest,
       should_name_be_the_name_passing_to_constructor) { // NOLINT
  ASSERT_EQ(expect_name, supplier->name());
}

TEST_F(GenericAttributeSupplierTest,
       should_max_attrs_be_the_max_attrs_passing_to_constructor) { // NOLINT
  ASSERT_EQ(expect_max_attrs, supplier->max_attrs());
}

BEGIN_MOCK_LISTENER_DEF(MockListener, IAttributeSupplier)

MOCK_METHOD1(AttributeChanged, void(IAttribute* attr));

BEGIN_BIND_SIGNAL(IAttributeSupplier)

BIND_SIGNAL1(AttributeChanged, void, IAttribute*, attr);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()


TEST_F(GenericAttributeSupplierTest,
       should_call_attributeChanged_emit_AttributeChanged_signal) { // NOLINT
  // Setup fixture
  auto attr = xtestutils::genDummyPointer<IAttribute>();

  // Expectations
  auto mock_listener = MockListener::attachTo(supplier.get());
  EXPECT_CALL(*mock_listener, AttributeChanged(attr));

  // Exercise system
  supplier->attributeChanged(attr);
}

TEST_F(GenericAttributeSupplierTest,
       should_be_able_to_add_attribute_and_call_createAttribute_of_sub_classes) { // NOLINT
  // Setup fixture
  auto expect_attr1 = new NullAttribute();
  auto expect_attr2 = new NullAttribute();
  auto expect_attr3 = new NullAttribute();

  std::vector<IAttribute*> expect_attr_vec;

  expect_attr_vec.push_back(expect_attr1);
  expect_attr_vec.push_back(expect_attr2);
  expect_attr_vec.push_back(expect_attr3);

  ASSERT_EQ(0, supplier->attr_count());

  // Expectations
  EXPECT_CALL(*supplier, createAttribute())
      .WillOnce(Return(expect_attr1))
      .WillOnce(Return(expect_attr2))
      .WillOnce(Return(expect_attr3));

  // Exercise system
  auto actual_attr1 = supplier->addAttribute();
  auto actual_attr2 = supplier->addAttribute();
  auto actual_attr3 = supplier->addAttribute();

  // Verify results
  ASSERT_EQ(expect_attr1, actual_attr1);
  ASSERT_EQ(expect_attr2, actual_attr2);
  ASSERT_EQ(expect_attr3, actual_attr3);
  ASSERT_EQ(3, supplier->attr_count());

  auto actual_attr_vec = supplier->attributes();
  ASSERT_EQ(expect_attr_vec, actual_attr_vec);
}

TEST_F(GenericAttributeSupplierTest,
       should_be_able_to_remove_and_destruct_existing_attr) { // NOLINT
  // Setup fixture
  auto expect_attr1 = new NullAttribute();
  auto expect_attr2 = new MockAttribute();
  auto expect_attr3 = new NullAttribute();

  std::vector<IAttribute*> expect_attr_vec;

  expect_attr_vec.push_back(expect_attr1);
  expect_attr_vec.push_back(expect_attr3);

  EXPECT_CALL(*supplier, createAttribute())
      .WillOnce(Return(expect_attr1))
      .WillOnce(Return(expect_attr2))
      .WillOnce(Return(expect_attr3));

  supplier->addAttribute();
  supplier->addAttribute();
  supplier->addAttribute();

  ASSERT_EQ(3, supplier->attr_count());

  // Expectations
  EXPECT_CALL(*expect_attr2, destroy());

  // Exercise system
  supplier->removeAttribute(expect_attr2);

  // Verify results
  ::Mock::VerifyAndClear(expect_attr2);
  ASSERT_EQ(2, supplier->attr_count());
  ASSERT_EQ(expect_attr_vec, supplier->attributes());
}

TEST_F(GenericAttributeSupplierTest,
       should_destroy_attributes_when_supplier_destroy) { // NOLINT
  // Setup fixture
  auto expect_attr1 = new MockAttribute();
  auto expect_attr2 = new MockAttribute();
  auto expect_attr3 = new MockAttribute();

  EXPECT_CALL(*supplier, createAttribute())
      .WillOnce(Return(expect_attr1))
      .WillOnce(Return(expect_attr2))
      .WillOnce(Return(expect_attr3));

  supplier->addAttribute();
  supplier->addAttribute();
  supplier->addAttribute();

  // Expectations
  EXPECT_CALL(*expect_attr1, destroy());
  EXPECT_CALL(*expect_attr2, destroy());
  EXPECT_CALL(*expect_attr3, destroy());

  // Exercise system
  supplier.reset();
}

}  // namespace tests
}  // namespace snailcore
