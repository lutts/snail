// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef GENERIC_ATTRIBUTE_SUPPLIER_TEST_H_
#define GENERIC_ATTRIBUTE_SUPPLIER_TEST_H_

#include "test/testutils/gmock_common.h"

namespace snailcore {
namespace tests {

class GenericAttributeFactoryFixture : public xtestutils::TestFixture {
 public:
  virtual void prepareCreateMockAttrs(int count) = 0;
  virtual std::vector<IAttribute*>& created_attributes() = 0;
};

template <typename F, typename M>
class AttributeFactoryFixtureTemplate : public GenericAttributeFactoryFixture {
 public:
  void prepareCreateMockAttrs(int count) override {
    // Expectations
    R_EXPECT_CALL(attr_factory_, createAttribute())
        .Times(count)
        .WillRepeatedly(Invoke([this]() {
          auto attr = new M();
          attr_vec_.push_back(attr);
          return attr;
        }));
  }

  std::vector<IAttribute*>& created_attributes() { return attr_vec_; }
  F* attr_factory() { return &attr_factory_; }

 private:
  F attr_factory_;
  std::vector<IAttribute*> attr_vec_;
};

class GenericAttributeSupplierFixture : public xtestutils::TestFixture {
 public:
  GenericAttributeSupplierFixture()
      : max_attrs_{xtestutils::randomIntInRange(3, 5)} {}

  // region: setter/getters
  int max_attrs() { return max_attrs_; }
  utils::U8String supplier_name() { return supplier_name_; }
  void set_supplier_name(const utils::U8String& name) { supplier_name_ = name; }

  void set_attr_supplier(IAttributeSupplier* attr_supplier) {
    attr_supplier_ = attr_supplier;
  }

  void set_attr_factory_fixture(
      GenericAttributeFactoryFixture* attr_factory_fixture) {
    attr_factory_fixture_ = attr_factory_fixture;
  }

  void prepareCreateMockAttrs(int count) {
    attr_factory_fixture_->prepareCreateMockAttrs(count);
  }

  std::vector<IAttribute*>& expect_attrs() {
    return attr_factory_fixture_->created_attributes();
  }
  // endregion: setter/getters

  // region: checkers
  void verify() override {
    xtestutils::TestFixture::verify();
    attr_factory_fixture_->verify();
  }

  virtual void validateState() {
    ASSERT_EQ(supplier_name_, attr_supplier_->name());
    ASSERT_EQ(max_attrs_, attr_supplier_->max_attrs());

    auto expect_attr_count = expect_attrs().size();
    ASSERT_EQ(expect_attr_count, attr_supplier_->attr_count())
        << "attr_count should be the attribute count currently created";

    ASSERT_EQ(expect_attrs(), attr_supplier_->attributes())
        << "attributes() should be a collection of the attributes currently "
           "created";
  }
  // endregion: checkers

  // region: additional state setup
  void fillAttributes() {
    prepareCreateMockAttrs(max_attrs_);
    for (int i = 0; i < max_attrs_; ++i) {
      attr_supplier_->addAttribute();
    }
  }
  // endregion: additional state setup

 private:
  utils::U8String supplier_name_{""};
  int max_attrs_;

  GenericAttributeFactoryFixture* attr_factory_fixture_;
  IAttributeSupplier* attr_supplier_{nullptr};

  friend class GenericAttributeSupplierFixtureWrapper;
};

class GenericAttributeSupplierFixtureWrapper {
 public:
  void setFixture(GenericAttributeSupplierFixture* fixture) {
    fixture_.reset(fixture);
    attr_supplier_ = fixture_->attr_supplier_;
  }

  void prepareCreateMockAttrs(int count) {
    fixture_->prepareCreateMockAttrs(count);
  }

  std::vector<IAttribute*>& expect_attrs() { return fixture_->expect_attrs(); }

  void validateState() { fixture_->validateState(); }

 protected:
  IAttributeSupplier* attr_supplier_;

 private:
  std::unique_ptr<GenericAttributeSupplierFixture> fixture_;
};

using GenericAttributeSupplierFixtureFactory =
    xtestutils::TestFixtureHelper<GenericAttributeSupplierFixture>;

class GenericAttributeSupplierWithMockAttrFactoryTest
    : public xtestutils::ErrorVerbosityTestWithParam<
          GenericAttributeSupplierFixtureFactory*>,
      public GenericAttributeSupplierFixtureWrapper {
 protected:
  GenericAttributeSupplierWithMockAttrFactoryTest()
      : ErrorVerbosityTestWithParam(), fixture_{this} {
    setFixture(fixture_.release());
  }

 private:
  xtestutils::FixtureLoaderFromHelper<
      GenericAttributeSupplierFixture,
      GenericAttributeSupplierWithMockAttrFactoryTest> fixture_;
};

TEST_P(GenericAttributeSupplierWithMockAttrFactoryTest,
       should_be_able_to_add_attribute) {  // NOLINT
  // Setup fixture
  const int kTestCount = 1;
  prepareCreateMockAttrs(kTestCount);

  // Exercise system
  attr_supplier_->addAttribute();

  // Verify results
  validateState();
}

class GenericAttributeSupplierFilledWithAttrsTest
    : public xtestutils::ErrorVerbosityTestWithParam<
          GenericAttributeSupplierFixtureFactory*>,
      public GenericAttributeSupplierFixtureWrapper {
 protected:
  GenericAttributeSupplierFilledWithAttrsTest()
      : ErrorVerbosityTestWithParam(), fixture_{this} {
    setFixture(fixture_.release());
  }

 private:
  xtestutils::FixtureLoaderFromHelper<
      GenericAttributeSupplierFixture,
      GenericAttributeSupplierFilledWithAttrsTest> fixture_;
};

TEST_P(
    GenericAttributeSupplierFilledWithAttrsTest,
    should_add_attribute_return_nullptr_and_not_call_createAttribute_when_attr_count_reach_max_attrs) {  // NOLINT
  // Exercise system
  auto new_attr = attr_supplier_->addAttribute();

  // Verify results
  ASSERT_EQ(nullptr, new_attr);
}

TEST_P(GenericAttributeSupplierFilledWithAttrsTest,
       should_be_able_to_remove_existing_attr) {  // NOLINT
  // Setup fixture
  auto& curr_attrs = expect_attrs();
  auto attr_to_remove =
      xtestutils::randomElementInContainer<IAttribute*>(curr_attrs);

  // Exercise system
  attr_supplier_->removeAttribute(attr_to_remove);

  // Verify results
  curr_attrs.erase(
      std::remove(curr_attrs.begin(), curr_attrs.end(), attr_to_remove),
      curr_attrs.end());
  validateState();
}

TEST_P(GenericAttributeSupplierFilledWithAttrsTest,
       should_call_attributeChanged_emit_AttributeChanged_signal) {  // NOLINT
  // Setup fixture
  auto attr = xtestutils::genDummyPointer<IAttribute>();

  // Expectations
  AttrSupplierListener supplier_listener(attr_supplier_);
  EXPECT_CALL(supplier_listener, AttributeChanged(attr));

  // Exercise system
  attr_supplier_->attributeChanged(attr);
}

}  // namespace tests
}  // namespace snailcore

#endif  // GENERIC_ATTRIBUTE_SUPPLIER_TEST_H_
