// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_ATTRIBUTE_SUPPLIER_H_
#define INCLUDE_CORE_I_KBNODE_ATTRIBUTE_SUPPLIER_H_

#include "core/generic_attribute_supplier.h"

namespace snailcore {

class IKbNode;

class KbNodeAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeAttributeSupplier() = default;

  virtual IKbNode* getRootKbNode() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_KBNODE_ATTRIBUTE_SUPPLIER_H_
