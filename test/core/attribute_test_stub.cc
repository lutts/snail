// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/core/attribute_test_stub.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {
namespace tests {

class AttrSupplierTestStubSignalProxy {
 public:
  SNAIL_SIGSLOT_PIMPL(AttrSupplierTestStub, AttributeChanged);

  friend class AttrSupplierTestStub;
};

SNAIL_SIGSLOT_DELEGATE(AttrSupplierTestStub, AttributeChanged, signal_proxy_);

AttrSupplierTestStub::AttrSupplierTestStub(const utils::U8String& name,
                                           int max_attrs)
    : signal_proxy_(utils::make_unique<AttrSupplierTestStubSignalProxy>()),
      name_(name),
      max_attrs_(max_attrs) {}

AttrSupplierTestStub::~AttrSupplierTestStub() {
  for (auto attr : attributes_) {
    delete attr;
  }
}

void AttrSupplierTestStub::attributeChanged(IAttribute* attr) {
  signal_proxy_->AttributeChanged(attr);
}

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

#define VERIFY_SUPPLIER(attrName) \
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
