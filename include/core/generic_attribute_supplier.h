// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_GENERIC_ATTRIBUTE_SUPPLIER_H_
#define INCLUDE_CORE_GENERIC_ATTRIBUTE_SUPPLIER_H_

#include <vector>

#include "snail/i_attribute_supplier.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

#include "snail/i_attribute.h"

namespace snailcore {

class GenericAttributeSupplier : public IAttributeSupplier {
 public:
  GenericAttributeSupplier(const utils::U8String& name, int max_attrs)
      : name_(name)
      , max_attrs_(max_attrs) { }
  virtual ~GenericAttributeSupplier() {
    for (auto attr : attrs_)
      delete attr;
  }

  utils::U8String name() const override {
    return name_;
  }

  int attr_count() const override {
    return attrs_.size();
  }

  std::vector<IAttribute*> attributes() const override {
    return attrs_;
  }

  int max_attrs() const override {
    return max_attrs_;
  }

  IAttribute* addAttribute() override {
    auto attr = createAttribute();
    attrs_.push_back(attr);

    return attr;
  }

  void removeAttribute(IAttribute* attr) override {
    auto iter = std::find(begin(attrs_), end(attrs_), attr);
    if (iter != end(attrs_)) {
      attrs_.erase(iter);
      delete attr;
    }
  }

  void attributeChanged(IAttribute* attr) override {
    AttributeChanged(attr);
  }

 private:
  virtual IAttribute* createAttribute() = 0;

 private:
  SNAIL_DISABLE_COPY(GenericAttributeSupplier);

 private:
  SNAIL_SIGSLOT_IMPL(AttributeChanged);

 private:
  utils::U8String name_;
  int max_attrs_;
  std::vector<IAttribute*> attrs_;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_GENERIC_ATTRIBUTE_SUPPLIER_H_
