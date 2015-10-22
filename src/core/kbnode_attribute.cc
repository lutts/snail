// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_attribute.h"
#include "snail/i_kbnode.h"

namespace snailcore {

KbNodeAttribute::KbNodeAttribute(KbNodeAttributeSupplier* attr_supplier)
    : GenericAttribute(attr_supplier)
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


// KbNodeAttribute
KbNodeAttributeSupplier* KbNodeAttribute::supplier() const {
  return attr_supplier_;
}

void KbNodeAttribute::setKbNode(IKbNode* kbnode) {
  kbnode_ = kbnode;
  dataChanged();
}

}  // namespace snailcore
