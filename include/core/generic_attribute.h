// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_GENERIC_ATTRIBUTE_H_
#define SRC_CORE_GENERIC_ATTRIBUTE_H_

#include "snail/i_attribute.h"
#include "snail/i_attribute_supplier.h"

namespace snailcore {

class GenericAttribute : public IAttribute {
 public:
  explicit GenericAttribute(IAttributeSupplier* supplier)
      : supplier_(supplier) { }
  virtual ~GenericAttribute() { }

  utils::U8String displayName() const override {
    return supplier_->name();
  }

 protected:
  void dataChanged() {
    supplier_->attributeChanged(this);
  }

  IAttributeSupplier* supplier() const {
    return supplier_;
  }

 private:
  IAttributeSupplier* supplier_;

 private:
  SNAIL_DISABLE_COPY(GenericAttribute)
};


}  // namespace snailcore

#endif  // SRC_CORE_GENERIC_ATTRIBUTE_H_
