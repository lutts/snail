// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_attribute_supplier.h"
#include "snail/i_kbnode.h"

namespace snailcore {

KbNodeAttributeSupplier::KbNodeAttributeSupplier(IKbNode* root_kbnode,
                                                 int max_attrs)
    : GenericAttributeSupplier(root_kbnode->name(), max_attrs)
    , root_kbnode_(root_kbnode) { }

KbNodeAttributeSupplier::~KbNodeAttributeSupplier() = default;

IAttribute* KbNodeAttributeSupplier::createAttribute() {
  //  return new KbNodeAttribute(this);
  return nullptr;  // TODO(lutts): remove this
}

IKbNode* KbNodeAttributeSupplier::getRootKbNode() const {
  return root_kbnode_;
}

}  // namespace snailcore
