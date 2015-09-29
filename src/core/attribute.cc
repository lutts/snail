// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/attribute.h"
#include "snail/i_kbnode.h"

namespace snailcore {

KbNodeAttribute::KbNodeAttribute(IKbNodeAttributeSupplier* attr_supplier)
    : IKbNodeAttribute(attr_supplier)
    , attr_supplier_(attr_supplier) { }
KbNodeAttribute::~KbNodeAttribute() = default;

// IAttribute
utils::U8String KbNodeAttribute::valueText() const {
  if (isEmpty())
    return "";

  return kbnode_->name();
}

bool KbNodeAttribute::isEmpty() const {
  return kbnode_ == nullptr;
}

void KbNodeAttribute::clear() {
  setKbNode(nullptr);
}


// IKbNodeAttribute
IKbNodeAttributeSupplier* KbNodeAttribute::kbnode_supplier() const {
  return attr_supplier_;
}

void KbNodeAttribute::setKbNode(IKbNode* kbnode) {
  kbnode_ = kbnode;
  dataChanged();
}

//////////////// KbNodeAttributeSupplier ////////////////

KbNodeAttributeSupplier::KbNodeAttributeSupplier(IKbNode* root_kbnode,
                                                 int max_attrs)
    : IKbNodeAttributeSupplier(root_kbnode->name(), max_attrs)
    , root_kbnode_(root_kbnode) { }

KbNodeAttributeSupplier::~KbNodeAttributeSupplier() = default;

IAttribute* KbNodeAttributeSupplier::createAttribute() {
  return new KbNodeAttribute(this);
}

IKbNode* KbNodeAttributeSupplier::getRootKbNode() const {
  return root_kbnode_;
}


}  // namespace snailcore
