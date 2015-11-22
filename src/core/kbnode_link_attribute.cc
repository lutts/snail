// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_link_attribute.h"
#include "src/core/generic_attribute_supplier.h"
#include "core/i_attribute_visitor.h"

namespace snailcore {

KbNodeLinkAttribute::KbNodeLinkAttribute(
    fto::KbNodeLinkAttributeSupplier* link_attr_supplier)
    : link_attr_supplier_(link_attr_supplier),
      link_type_(*link_attr_supplier->getDefaultProtoLinkType()),
      value_attr_supplier_(link_attr_supplier->getRootKbNode(), 1) {
  link_type_.whenLinkUpdated([this]() { linkUpdated(); }, nullptr);
  value_attr_supplier_.whenAttributeChanged([this](IAttribute* attr) {
    (void)attr;
    emitAttributeChanged();
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

void KbNodeLinkAttribute::emitAttributeChanged() {
  link_attr_supplier_->attributeChanged(this);
}

void KbNodeLinkAttribute::linkUpdated() { emitAttributeChanged(); }

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
  if (value_attr_) value_attr_->clear();
  link_type_.clear();
}

void KbNodeLinkAttribute::accept(IAttributeVisitor* visitor) {
  visitor->visit(this);
}

fto::KbNodeLinkAttributeSupplier* KbNodeLinkAttribute::supplier() const {
  return link_attr_supplier_;
}

void KbNodeLinkAttribute::initValueAttr() {
  if (value_attr_) return;

  IAttribute* attr;
  if (value_attr_supplier_.attr_count() == 0) {
    attr = value_attr_supplier_.addAttribute();
  } else {
    auto attrs = value_attr_supplier_.attributes();
    attr = attrs[0];
  }

  value_attr_ = static_cast<fto::KbNodeAttribute*>(attr);
}

fto::KbNodeAttribute* KbNodeLinkAttribute::valueAttr() {
  initValueAttr();
  return value_attr_;
}

fto::LinkType* KbNodeLinkAttribute::linkType() { return link_type_.self(); }

//////////////// KbNodeLinkAttributeSupplier impls ////////////////

class KbNodeLinkAttributeSupplierPrivate
    : public GenericAttributeSupplier<KbNodeLinkAttributeSupplier,
                                      KbNodeLinkAttribute> {
 public:
  KbNodeLinkAttributeSupplierPrivate(KbNodeLinkAttributeSupplier* q_ptr,
                                     int max_attrs)
      : GenericAttributeSupplier{q_ptr, "", max_attrs} {}

 private:
};

KbNodeLinkAttributeSupplier::KbNodeLinkAttributeSupplier(
    ITreeItemProvider* link_type_item_provider,
    const fto::LinkType* default_proto_link_type, IKbNode* root_kbnode,
    int max_attrs)
    : impl_(utils::make_unique<KbNodeLinkAttributeSupplierPrivate>(this,
                                                                   max_attrs)),
      link_type_item_provider_(link_type_item_provider),
      default_proto_link_type_(default_proto_link_type),
      root_kbnode_(root_kbnode) {}

KbNodeLinkAttributeSupplier::~KbNodeLinkAttributeSupplier() = default;

SNAIL_SIGSLOT_DELEGATE(KbNodeLinkAttributeSupplier, AttributeChanged, impl_);

utils::U8String KbNodeLinkAttributeSupplier::name() const {
  return impl_->name();
}
int KbNodeLinkAttributeSupplier::attr_count() const {
  return impl_->attr_count();
}
std::vector<IAttribute*> KbNodeLinkAttributeSupplier::attributes() const {
  return impl_->attributes();
}
int KbNodeLinkAttributeSupplier::max_attrs() const {
  return impl_->max_attrs();
}
IAttribute* KbNodeLinkAttributeSupplier::addAttribute() {
  return impl_->addAttribute();
}
void KbNodeLinkAttributeSupplier::removeAttribute(IAttribute* attr) {
  impl_->removeAttribute(attr);
}
void KbNodeLinkAttributeSupplier::attributeChanged(IAttribute* attr) {
  impl_->attributeChanged(attr);
}

}  // namespace snailcore
