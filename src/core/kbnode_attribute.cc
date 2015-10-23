// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_attribute.h"
#include "snail/i_kbnode.h"
#include "core/i_attribute_visitor.h"

namespace snailcore {

KbNodeAttribute::KbNodeAttribute(fto::KbNodeAttributeSupplier* attr_supplier)
    : attr_supplier_(attr_supplier) { }
KbNodeAttribute::~KbNodeAttribute() = default;

// IAttribute
utils::U8String KbNodeAttribute::displayName() const {
  return attr_supplier_->name();
}

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

void KbNodeAttribute::accept(IAttributeVisitor* visitor) {
  visitor->visit(this);
}


// IKbNodeAttribute
fto::KbNodeAttributeSupplier* KbNodeAttribute::supplier() const {
  return attr_supplier_;
}

void KbNodeAttribute::setKbNode(IKbNode* kbnode) {
  kbnode_ = kbnode;
  attr_supplier_->attributeChanged(this);
}

//////////////// KbNodeAttributeSupplier ////////////////

KbNodeAttributeSupplier::KbNodeAttributeSupplier(IKbNode* root_kbnode,
                                                 int max_attrs)
    : FTO_INTERFACE(KbNodeAttributeSupplier)(root_kbnode->name(), max_attrs)
    , root_kbnode_(root_kbnode) { }

KbNodeAttributeSupplier::~KbNodeAttributeSupplier() = default;

IAttribute* KbNodeAttributeSupplier::createAttribute() {
  return new KbNodeAttribute(this);
}

IKbNode* KbNodeAttributeSupplier::getRootKbNode() const {
  return root_kbnode_;
}


}  // namespace snailcore
