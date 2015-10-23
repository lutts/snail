// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_

#include "snail/i_attribute.h"
#include "core/i_kbnode_attribute_supplier.h"

namespace snailcore {

class IKbNode;

class IKbNodeAttribute : public IAttribute {
 public:
  virtual ~IKbNodeAttribute() = default;

  virtual IKbNodeAttributeSupplier* supplier() const = 0;
  virtual void setKbNode(IKbNode* kbnode) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_
