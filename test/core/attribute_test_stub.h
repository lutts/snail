// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_CORE_ATTRIBUTE_TEST_STUB_H_
#define TEST_CORE_ATTRIBUTE_TEST_STUB_H_

#include "utils/basic_utils.h"
#include "snail/i_attribute.h"
#include "snail/i_attribute_supplier.h"

namespace snailcore {
namespace tests {

class AttributeTestStub :public IAttribute {
 public:
  AttributeTestStub(const utils::U8String& name,
                    const utils::U8String& value,
                    IAttributeSupplier* supplier)
      : name_(name)
      , value_(value)
      , supplier_(supplier) { }
  virtual ~AttributeTestStub() = default;

  utils::U8String displayName() const override {
    return name_;
  }

  void setDisplayName(const utils::U8String& name) {
    name_ = name;
    supplier_->attributeChanged(this);
  }

  utils::U8String valueText() const override {
    return value_;
  }

  void setValueText(const utils::U8String& value) {
    value_ = value;
  }

  bool isEmpty() const override {
    return value_.empty();
  }

  void clear() override {
    value_.clear();
  }

 private:
  SNAIL_DISABLE_COPY(AttributeTestStub);

  utils::U8String name_;
  utils::U8String value_;
  IAttributeSupplier* supplier_;
};

class AttrSupplierTestStub : public IAttributeSupplier {
 public:
  AttrSupplierTestStub(const utils::U8String& name,
                       int max_attrs)
      : name_(name), max_attrs_(max_attrs) { }
  virtual ~AttrSupplierTestStub() {
    for (auto attr : attributes_) {
      delete attr;
    }
  }

  utils::U8String name() const override {
    return name_;
  }

  int max_attrs() const override {
    return max_attrs_;
  }

  int attr_count() const override {
    return attributes_.size();
  }

  std::vector<IAttribute*> attributes() const override {
    return attributes_;
  }

  IAttribute* addAttribute() override {
    auto attr = addAttribute(name_, "");
    addAttributeCalled(attr);
    return attr;
  }

  void removeAttribute(IAttribute* attr) override {
    auto iter = std::find(begin(attributes_),
                          end(attributes_),
                          attr);
    if (iter != end(attributes_)) {
      attributes_.erase(iter);
      attributeRemoved(attr);
    }
  }

  AttributeTestStub* addAttribute(const utils::U8String& attr_name,
                                  const utils::U8String& attr_value) {
    if (attributes_.size() > static_cast<size_t>(max_attrs_))
      return nullptr;

    auto attr = new AttributeTestStub(attr_name, attr_value, this);
    attributes_.push_back(attr);

    return attr;
  }

  void attributeChanged(IAttribute* attr) override {
    AttributeChanged(attr);
  }

  virtual ComplexReturnValue addAttributeCalled(IAttribute* new_attr) = 0;
  virtual ComplexReturnValue attributeRemoved(IAttribute* attr) = 0;

 private:
  SNAIL_SIGSLOT_IMPL(AttributeChanged);

 private:
  SNAIL_DISABLE_COPY(AttrSupplierTestStub);

  utils::U8String name_;
  int max_attrs_;
  std::vector<IAttribute*> attributes_;
};

class MockAttrSupplierTestStub : public AttrSupplierTestStub {
 public:
  MockAttrSupplierTestStub(const utils::U8String& name,
                           int max_attrs)
      : AttrSupplierTestStub(name, max_attrs) { }
  virtual ~MockAttrSupplierTestStub() = default;

  MOCK_METHOD1(addAttributeCalled, ComplexReturnValue(IAttribute* new_attr));
  MOCK_METHOD1(attributeRemoved, ComplexReturnValue(IAttribute* attr));

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

std::vector<std::unique_ptr<MockAttrSupplierTestStub> >
MockAttrSupplierTestStub::createAttributeSuppliers() {
  std::vector<std::unique_ptr<MockAttrSupplierTestStub> > attr_suppliers;
  std::unique_ptr<MockAttrSupplierTestStub> attr_supplier;

  attr_supplier = utils::make_unique<MockAttrSupplierTestStub>(
      "Supplier_Max_Eq_1_AttrCount_Eq_0", 1);
  attr_suppliers.push_back(std::move(attr_supplier));

  attr_supplier = utils::make_unique<MockAttrSupplierTestStub>(
      "Supplier_Max_Eq_1_AttrCount_Eq_1", 1);
  attr_supplier->addAttribute("Attribute 01", "Value 1");
  attr_suppliers.push_back(std::move(attr_supplier));

  attr_supplier = utils::make_unique<MockAttrSupplierTestStub>(
      "Supplier_Max_Gt_1_AttrCount_Eq_0", 3);
  attr_suppliers.push_back(std::move(attr_supplier));

  attr_supplier = utils::make_unique<MockAttrSupplierTestStub>(
      "Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1", 3);
  attr_supplier->addAttribute("Attribute 02", "Value 2");
  attr_supplier->addAttribute("Attribute 03", "Value 3");
  attr_suppliers.push_back(std::move(attr_supplier));

  attr_supplier = utils::make_unique<MockAttrSupplierTestStub>(
      "Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2", 3);
  attr_supplier->addAttribute("Attribute 04", "Value 4");
  attr_suppliers.push_back(std::move(attr_supplier));

  attr_supplier = utils::make_unique<MockAttrSupplierTestStub>(
      "Supplier_Max_Gt_1_AttrCount_Eq_Max", 3);
  attr_supplier->addAttribute("Attribute 05", "Value 5");
  attr_supplier->addAttribute("Attribute 06", "Value 6");
  // NOTE: this attr's name is same as next supplier's first attr name
  attr_supplier->addAttribute("Attribute 07", "Value 7");
  attr_suppliers.push_back(std::move(attr_supplier));

  attr_supplier = utils::make_unique<MockAttrSupplierTestStub>(
      "Supplier_With_Duplicated_Named_Attrs", 4);
  attr_supplier->addAttribute("Attribute 07", "Value 7-1");
  attr_supplier->addAttribute("Attribute 08", "Value 8-1");
  attr_supplier->addAttribute("Attribute 07", "Value 7-2");
  attr_supplier->addAttribute("Attribute 08", "Value 8-2");
  attr_suppliers.push_back(std::move(attr_supplier));

  attr_supplier = utils::make_unique<MockAttrSupplierTestStub>(
      "Supplier_With_Attr_Name_Same_As_Previous_Supplier", 1);
  attr_supplier->addAttribute("Attribute 08", "Value 8-3");
  attr_suppliers.push_back(std::move(attr_supplier));

#define VERIFY_SUPPLIER(attrName)                       \
  ASSERT_EQ(attr_suppliers[kIdx_##attrName]->name(), #attrName);

  auto verifier = [&attr_suppliers]() {
    VERIFY_SUPPLIER(Supplier_Max_Eq_1_AttrCount_Eq_0);
    VERIFY_SUPPLIER(Supplier_Max_Eq_1_AttrCount_Eq_1);
    VERIFY_SUPPLIER(Supplier_Max_Gt_1_AttrCount_Eq_0);
    VERIFY_SUPPLIER(Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1);
    VERIFY_SUPPLIER(Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2);
    VERIFY_SUPPLIER(Supplier_Max_Gt_1_AttrCount_Eq_Max);
    VERIFY_SUPPLIER(Supplier_With_Duplicated_Named_Attrs);
    VERIFY_SUPPLIER(Supplier_With_Attr_Name_Same_As_Previous_Supplier);
  };

  verifier();

  return attr_suppliers;
}

}  // namespace tests
}  // namespace snailcore

#endif  // TEST_CORE_ATTRIBUTE_TEST_STUB_H_
