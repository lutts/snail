// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_GENERIC_ATTRIBUTE_H_
#define INCLUDE_CORE_GENERIC_ATTRIBUTE_H_

#include "snail/i_attribute.h"
#include "snail/i_attribute_supplier.h"

namespace snailcore {

class IAttributeVisitor;

class GenericAttribute : public IAttribute {
 public:
  explicit GenericAttribute(IAttributeSupplier* supplier)
      : supplier_(supplier) { }
  virtual ~GenericAttribute() { }

  utils::U8String displayName() const override {
    return supplier_->name();
  }

  virtual void accept(IAttributeVisitor* visitor) = 0;

 protected:
  void dataChanged() {
    supplier_->attributeChanged(this);
  }

 private:
  IAttributeSupplier* supplier_;

 private:
  SNAIL_DISABLE_COPY(GenericAttribute)
};


}  // namespace snailcore

#endif  // INCLUDE_CORE_GENERIC_ATTRIBUTE_H_
