// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_CORE_ATTRIBUTE_TEST_STUB_H_
#define TEST_CORE_ATTRIBUTE_TEST_STUB_H_

#include <memory>
#include <vector>

#include "test/testutils/gmock_common.h"
#include "utils/basic_utils.h"
#include "snail/i_attribute.h"
#include "snail/i_attribute_supplier.h"

namespace snailcore {
namespace tests {

class AttributeTestStub : public IAttribute {
 public:
  AttributeTestStub(const utils::U8String& name, const utils::U8String& value,
                    IAttributeSupplier* supplier)
      : name_(name), value_(value), supplier_(supplier) {}
  virtual ~AttributeTestStub() = default;

  utils::U8String displayName() const override { return name_; }

  void setDisplayName(const utils::U8String& name) {
    name_ = name;
    supplier_->attributeChanged(this);
  }

  utils::U8String valueText() const override { return value_; }

  void setValueText(const utils::U8String& value) { value_ = value; }

  bool isEmpty() const override { return value_.empty(); }

  void clear() override { value_.clear(); }

 private:
  SNAIL_DISABLE_COPY(AttributeTestStub);

  MOCK_METHOD1(accept, void(IAttributeVisitor* visitor));

  utils::U8String name_;
  utils::U8String value_;
  IAttributeSupplier* supplier_;
};

class AttrSupplierTestStubSignalProxy;

class AttrSupplierTestStub : public IAttributeSupplier {
 public:
  AttrSupplierTestStub(const utils::U8String& name, int max_attrs);
  virtual ~AttrSupplierTestStub();

  utils::U8String name() const override { return name_; }

  int max_attrs() const override { return max_attrs_; }

  int attr_count() const override { return attributes_.size(); }

  std::vector<IAttribute*> attributes() const override { return attributes_; }

  IAttribute* addAttribute() override {
    auto attr = addAttribute(name_, "");
    addAttributeCalled(attr);
    return attr;
  }

  void removeAttribute(IAttribute* attr) override {
    auto iter = std::find(begin(attributes_), end(attributes_), attr);
    if (iter != end(attributes_)) {
      attributes_.erase(iter);
      attributeRemoved(attr);
    }
  }

  AttributeTestStub* addAttribute(const utils::U8String& attr_name,
                                  const utils::U8String& attr_value) {
    if (attributes_.size() > static_cast<size_t>(max_attrs_)) return nullptr;

    auto attr = new AttributeTestStub(attr_name, attr_value, this);
    attributes_.push_back(attr);

    return attr;
  }

  void attributeChanged(IAttribute* attr) override;

  virtual ComplexReturnValue addAttributeCalled(IAttribute* new_attr) = 0;
  virtual ComplexReturnValue attributeRemoved(IAttribute* attr) = 0;

 public:
  SNAIL_SIGSLOT_NONVIRTUAL(AttributeChanged, void(IAttribute* attr));

 private:
  std::unique_ptr<AttrSupplierTestStubSignalProxy> signal_proxy_;

 private:
  SNAIL_DISABLE_COPY(AttrSupplierTestStub);

  utils::U8String name_;
  int max_attrs_;
  std::vector<IAttribute*> attributes_;
};

class MockAttrSupplierTestStub : public AttrSupplierTestStub {
 public:
  MockAttrSupplierTestStub()
      : MockAttrSupplierTestStub{xtestutils::genRandomString(),
                                 std::rand() % 5} {}
  MockAttrSupplierTestStub(const utils::U8String& name, int max_attrs)
      : AttrSupplierTestStub(name, max_attrs) {}
  virtual ~MockAttrSupplierTestStub() = default;

  MOCK_METHOD1(addAttributeCalled, ComplexReturnValue(IAttribute* new_attr));
  MOCK_METHOD1(attributeRemoved, ComplexReturnValue(IAttribute* attr));

  MOCK_CONST_METHOD0(clone, IAttributeSupplier*());

  static std::vector<std::unique_ptr<MockAttrSupplierTestStub>>
  createAttributeSuppliers();

  enum {
    kIdx_Supplier_Max_Eq_1_AttrCount_Eq_0,
    kIdx_Supplier_Max_Eq_1_AttrCount_Eq_1,
    kIdx_Supplier_Max_Gt_1_AttrCount_Eq_0,
    kIdx_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1,
    kIdx_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2,
    kIdx_Supplier_Max_Gt_1_AttrCount_Eq_Max,
    kIdx_Supplier_With_Duplicated_Named_Attrs,
    kIdx_Supplier_With_Attr_Name_Same_As_Previous_Supplier,
  };
};

}  // namespace tests
}  // namespace snailcore

#endif  // TEST_CORE_ATTRIBUTE_TEST_STUB_H_
