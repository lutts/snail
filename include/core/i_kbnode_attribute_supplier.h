// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_ATTRIBUTE_SUPPLIER_H_
#define INCLUDE_CORE_I_KBNODE_ATTRIBUTE_SUPPLIER_H_

#include "src/core/generic_attribute_supplier.h"
#include "core/kb_global.h"

namespace snailcore {

class IKbNodeAttributeSupplier : public GenericAttributeSupplier {
 public:
  IKbNodeAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~IKbNodeAttributeSupplier() = default;

  virtual KbNodeIdType getRootKbNodeId() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_KBNODE_ATTRIBUTE_SUPPLIER_H_
