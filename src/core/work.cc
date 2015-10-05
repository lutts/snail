// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/work.h"

namespace snailcore {

Work::Work() = default;
Work::~Work() = default;

bool Work::set_name(const utils::U8String& new_name) {
  if (this->name_ != new_name) {
    name_ = new_name;
    NameChanged(name_);
    return true;
  }

  return false;
}

const utils::U8String& Work::name() const {
  return name_;
}

void Work::setAttributeSuppliers(
    std::vector<std::unique_ptr<IAttributeSupplier> >&& attr_suppliers) {
  attr_suppliers_ = std::move(attr_suppliers);
}

std::vector<IAttributeSupplier*> Work::attributeSuppliers() const {
  std::vector<IAttributeSupplier*> attr_suppliers;
  for (auto & attr_supplier : attr_suppliers_)
    attr_suppliers.push_back(attr_supplier.get());

  return attr_suppliers;
}

}  // namespace snailcore
