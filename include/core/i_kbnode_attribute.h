// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_

#include "snail/i_attribute.h"
#include "core/generic_attribute_supplier.h"

namespace snailcore {

class IKbNode;
class IKbNodeAttributeSupplier;

class IKbNodeAttribute : public IAttribute {
 public:
  virtual ~IKbNodeAttribute() = default;

  virtual IKbNodeAttributeSupplier* supplier() const = 0;
  virtual void setKbNode(IKbNode* kbnode) = 0;
};

class IKbNodeAttributeSupplier : public GenericAttributeSupplier {
 public:
  IKbNodeAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~IKbNodeAttributeSupplier() = default;

  virtual IKbNode* getRootKbNode() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_KBNODE_ATTRIBUTE_H_
