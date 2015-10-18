// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_

#include "core/generic_attribute.h"
#include "core/i_kbnode_attribute_supplier.h"
#include "core/i_attribute_visitor.h"

namespace snailcore {

class IKbNode;

class IKbNodeAttribute : public GenericAttribute {
 public:
  explicit IKbNodeAttribute(IKbNodeAttributeSupplier* supplier)
      : GenericAttribute(supplier) { }
  virtual ~IKbNodeAttribute() = default;

  void accept(IAttributeVisitor* visitor) override {
    visitor->visit(this);
  }

  virtual IKbNodeAttributeSupplier* supplier() const = 0;
  virtual void setKbNode(IKbNode* kbnode) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_
