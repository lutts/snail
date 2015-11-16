// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_link_attribute.h"
#include "core/i_attribute_visitor.h"

namespace snailcore {

KbNodeLinkAttribute::KbNodeLinkAttribute(
    fto::KbNodeLinkAttributeSupplier* link_attr_supplier)
    : link_attr_supplier_(link_attr_supplier)
    , link_type_(*link_attr_supplier->getDefaultProtoLinkType())
    , value_attr_supplier_(link_attr_supplier->getRootKbNode(), 1) {
  link_type_.whenLinkUpdated(
      [this]() {
        linkUpdated();
      }, nullptr);
}

KbNodeLinkAttribute::~KbNodeLinkAttribute() = default;

utils::U8String KbNodeLinkAttribute::displayName() const {
  auto link_name = link_type_.toString();
  if (link_name.empty())
    return value_attr_supplier_.name();
  else
    return link_name;
}

void KbNodeLinkAttribute::linkUpdated() {
  link_attr_supplier_->attributeChanged(this);
}

utils::U8String KbNodeLinkAttribute::valueText() const {
  auto that = const_cast<KbNodeLinkAttribute*>(this);
  that->initValueAttr();

  if (value_attr_)
    return value_attr_->valueText();
  else
    return "";
}

bool KbNodeLinkAttribute::isEmpty() const {
  auto that = const_cast<KbNodeLinkAttribute*>(this);
  that->initValueAttr();

  if (value_attr_)
    return value_attr_->isEmpty();
  else
    return true;
}

void KbNodeLinkAttribute::clear() {
  if (value_attr_)
    value_attr_->clear();
  link_type_.clear();
}

void KbNodeLinkAttribute::accept(IAttributeVisitor* visitor) {
  visitor->visit(this);
}

fto::KbNodeLinkAttributeSupplier* KbNodeLinkAttribute::supplier() const {
  return link_attr_supplier_;
}

void KbNodeLinkAttribute::initValueAttr() {
  if (value_attr_)
    return;

  IAttribute* attr;
  if (value_attr_supplier_.attr_count() == 0) {
    attr = value_attr_supplier_.addAttribute();
  } else {
    auto attrs = value_attr_supplier_.attributes();
    attr =  attrs[0];
  }

  value_attr_ = static_cast<fto::KbNodeAttribute*>(attr);
}

fto::KbNodeAttribute* KbNodeLinkAttribute::valueAttr() {
  initValueAttr();
  return value_attr_;
}

fto::LinkType* KbNodeLinkAttribute::linkType() {
  return link_type_.self();
}

IAttribute* KbNodeLinkAttributeSupplier::createAttribute() {
  return new KbNodeLinkAttribute(this);
}

}  // namespace snailcore
