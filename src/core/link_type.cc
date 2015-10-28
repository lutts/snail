// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/link_type.h"

namespace snailcore {

LinkType::LinkType() : LinkType("", false) { }

LinkType::LinkType(const utils::U8String& name,
                   bool is_group_only)
    : name_(name)
    , is_group_only_(is_group_only) { }

LinkType::LinkType(const LinkType& rhs) {
  name_ = rhs.name_;
  is_group_only_ = rhs.is_group_only_;

  for (auto & supplier : rhs.attr_suppliers_) {
    std::unique_ptr<IAttributeSupplier> supplier_clone(supplier->clone());
    attr_suppliers_.push_back(std::move(supplier_clone));
  }
}

LinkType::~LinkType() = default;

utils::U8String LinkType::name() const {
  return name_;
}

bool LinkType::isGroupOnly() const {
  return is_group_only_;
}

void LinkType::setAttributeSuppliers(
    std::vector<std::unique_ptr<IAttributeSupplier>>&& attr_suppliers) {
  attr_suppliers_ = std::move(attr_suppliers);
}

std::vector<IAttributeSupplier*> LinkType::attributeSuppliers() const {
  std::vector<IAttributeSupplier*> suppliers;

  for (auto & supplier : attr_suppliers_) {
    suppliers.push_back(supplier.get());
  }

  return suppliers;
}

LinkType& LinkType::operator=(LinkType rhs) {
  rhs.swap(*this);
  return *this;
}

void LinkType::swap(LinkType& rhs) noexcept {
  std::swap(name_, rhs.name_);
  std::swap(is_group_only_, rhs.is_group_only_);
  std::swap(attr_suppliers_, rhs.attr_suppliers_);
}

}  // namespace snailcore
