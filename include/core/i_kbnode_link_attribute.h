// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_LINK_ATTRIBUTE_H_
#define I_KBNODE_LINK_ATTRIBUTE_H_

#include "core/generic_attribute.h"
#include "core/i_kbnode_link_attribute_supplier.h"
#include "core/i_attribute_visitor.h"

namespace snailcore {

class IKbNodeLinkAttribute : public GenericAttribute {
 public:
  IKbNodeLinkAttribute(IKbNodeLinkAttributeSupplier* supplier)
      : GenericAttribute(supplier) { }
  virtual ~IKbNodeLinkAttribute() = default;

  void accept(IAttributeVisitor* visitor) override {
    (void)visitor;
    // TODO(lutts): impl this method
  }
};

}  // namespace snailcore

#endif  // I_KBNODE_LINK_ATTRIBUTE_H_
