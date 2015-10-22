// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_

#include "core/generic_attribute.h"
#include "core/kbnode_attribute_supplier.h"
#include "core/i_attribute_visitor.h"

namespace snailcore {

class IKbNode;

class KbNodeAttribute : public GenericAttribute {
 public:
  explicit KbNodeAttribute(KbNodeAttributeSupplier* supplier)
      : GenericAttribute(supplier) { }
  virtual ~KbNodeAttribute() = default;

  void accept(IAttributeVisitor* visitor) override {
    visitor->visit(this);
  }

  virtual KbNodeAttributeSupplier* supplier() const = 0;
  virtual void setKbNode(IKbNode* kbnode) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_
