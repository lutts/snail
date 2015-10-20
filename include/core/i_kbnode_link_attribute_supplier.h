// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_LINK_ATTRIBUTE_SUPPLIER_H_
#define I_KBNODE_LINK_ATTRIBUTE_SUPPLIER_H_

#include "core/generic_attribute_supplier.h"

namespace snailcore {

class ITreeItemProvider;

class IKbNodeLinkAttributeSupplier : public GenericAttributeSupplier {
 public:
  IKbNodeLinkAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~IKbNodeLinkAttributeSupplier() = default;

  virtual ITreeItemProvider* getLinkTypeProvider() const = 0;
};

}  // namespace snailcore

#endif  // I_KBNODE_LINK_ATTRIBUTE_SUPPLIER_H_
