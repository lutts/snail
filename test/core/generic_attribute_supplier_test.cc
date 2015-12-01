//-*- TestCaseName: GenericAttributeSupplierTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/generic_attribute_supplier.h"
#include "snail/mock_attribute.h"
#include "snail/mock_attribute_supplier.h"

namespace snailcore {
namespace tests {

class MockGenericAttributeSupplier
    : public GenericAttributeSupplier<MockAttributeSupplier, MockAttribute> {
 public:
  MockGenericAttributeSupplier(MockAttributeSupplier* actual_supplier,
                               const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(actual_supplier, name, max_attrs) {
    ON_CALL(*this, customCreateAttribute(actual_supplier))
        .WillByDefault(Return(nullptr));
  }

  MockAttribute* createAttribute(
      MockAttributeSupplier* actual_supplier) override {
    auto attr = customCreateAttribute(actual_supplier);
    if (attr) {
      return attr;
    } else {
      return GenericAttributeSupplier::createAttribute(actual_supplier);
    }
  }

  MOCK_METHOD1(customCreateAttribute,
               MockAttribute*(MockAttributeSupplier* actual_supplier));
};

class GenericAttrSupplierListener
    : public xtestutils::SimpleMockListener<MockGenericAttributeSupplier> {
 public:
  SNAIL_MOCK_LISTENER1(GenericAttrSupplierListener, AttributeChanged,
                       void(IAttribute* attr));

  explicit GenericAttrSupplierListener(MockGenericAttributeSupplier* subject)
      : SimpleMockListener(subject) {
    SNAIL_MOCK_LISTENER_REGISTER(AttributeChanged, this);

    attach();
  }

  ~GenericAttrSupplierListener() { detatch(); }
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
    actual_supplier = xtestutils::genDummyPointer<MockAttributeSupplier>();

    supplier = utils::make_unique<MockGenericAttributeSupplier>(
        actual_supplier, expect_name, expect_max_attrs);
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  utils::U8String expect_name;
  int expect_max_attrs;

  MockAttributeSupplier* actual_supplier;
  // endregion

  // region: test subject
  std::unique_ptr<MockGenericAttributeSupplier> supplier;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(GenericAttributeSupplierTest,
       should_name_be_the_name_passing_to_constructor) {  // NOLINT
  ASSERT_EQ(expect_name, supplier->name());
}

TEST_F(GenericAttributeSupplierTest,
       should_max_attrs_be_the_max_attrs_passing_to_constructor) {  // NOLINT
  ASSERT_EQ(expect_max_attrs, supplier->max_attrs());
}

TEST_F(GenericAttributeSupplierTest,
       should_call_attributeChanged_emit_AttributeChanged_signal) {  // NOLINT
  // Setup fixture
  auto attr = xtestutils::genDummyPointer<IAttribute>();

  // Expectations
  GenericAttrSupplierListener supplier_listener(supplier.get());
  EXPECT_CALL(supplier_listener, AttributeChanged(attr));

  // Exercise system
  supplier->attributeChanged(attr);
}

TEST_F(GenericAttributeSupplierTest,
       should_be_able_to_by_default_create_MockAttributes) {  // NOLINT
  // Setup fixture
  ASSERT_EQ(0, supplier->attr_count());

  std::vector<IAttribute*> expect_attr_vec;

  // Expectations

  // Exercise system
  auto actual_attr1 = supplier->addAttribute();
  auto actual_attr2 = supplier->addAttribute();
  auto actual_attr3 = supplier->addAttribute();

  // Verify results
  expect_attr_vec.push_back(actual_attr1);
  expect_attr_vec.push_back(actual_attr2);
  expect_attr_vec.push_back(actual_attr3);

  ASSERT_EQ(3, supplier->attr_count());

  auto actual_attr_vec = supplier->attributes();
  ASSERT_EQ(expect_attr_vec, actual_attr_vec);
}

TEST_F(GenericAttributeSupplierTest,
       should_be_able_to_create_custom_attributes_by_sub_classes) {  // NOLINT
  // Setup fixture
  auto expect_attr1 = new MockAttribute();
  auto expect_attr2 = new MockAttribute();
  auto expect_attr3 = new MockAttribute();

  std::vector<IAttribute*> expect_attr_vec;

  expect_attr_vec.push_back(expect_attr1);
  expect_attr_vec.push_back(expect_attr2);
  expect_attr_vec.push_back(expect_attr3);

  ASSERT_EQ(0, supplier->attr_count());

  // Expectations
  EXPECT_CALL(*supplier, customCreateAttribute(actual_supplier))
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

TEST_F(
    GenericAttributeSupplierTest,
    should_not_add_attribute_and_not_call_createAttribute_of_sub_class_when_attrcount_eq_max_attrs) {  // NOLINT
  // Setup fixture
  const int max_attrs = 2;
  MockGenericAttributeSupplier full_test_supplier{actual_supplier, "",
                                                  max_attrs};
  for (int i = 0; i < max_attrs; ++i) full_test_supplier.addAttribute();

  ASSERT_EQ(max_attrs, full_test_supplier.attr_count());

  // Expectations
  EXPECT_CALL(full_test_supplier, customCreateAttribute(actual_supplier))
      .Times(0);

  // Exercise system
  auto actual_attr = full_test_supplier.addAttribute();

  // Verify results
  ASSERT_EQ(nullptr, actual_attr);
}

TEST_F(GenericAttributeSupplierTest,
       should_be_able_to_remove_and_destruct_existing_attr) {  // NOLINT
  // Setup fixture
  auto expect_attr1 = new MockAttribute();
  auto expect_attr2 = new MockAttribute();
  auto expect_attr3 = new MockAttribute();

  std::vector<IAttribute*> expect_attr_vec;

  expect_attr_vec.push_back(expect_attr1);
  expect_attr_vec.push_back(expect_attr3);

  EXPECT_CALL(*supplier, customCreateAttribute(actual_supplier))
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
       should_destroy_attributes_when_supplier_destroy) {  // NOLINT
  // Setup fixture
  auto expect_attr1 = new MockAttribute();
  auto expect_attr2 = new MockAttribute();
  auto expect_attr3 = new MockAttribute();

  EXPECT_CALL(*supplier, customCreateAttribute(actual_supplier))
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
