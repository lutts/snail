// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_link_attribute.h"

#include <boost/polymorphic_cast.hpp>

#include <vector>

#include "utils/compiler.h"
#include "src/core/generic_attribute_supplier.h"
#include "core/i_attribute_visitor.h"

namespace snailcore {

KbNodeLinkAttribute::KbNodeLinkAttribute(
    fto::KbNodeLinkAttributeSupplier* link_attr_supplier)
    : link_attr_supplier_(link_attr_supplier),
      link_type_(*link_attr_supplier_->getDefaultProtoLinkType()),
      value_attr_supplier_(link_attr_supplier_->getRootKbNode(), 1) {
  connectSignals();
}

KbNodeLinkAttribute::KbNodeLinkAttribute(const KbNodeLinkAttribute& rhs)
    : link_attr_supplier_{rhs.link_attr_supplier_},
      link_type_{rhs.link_type_},
      value_attr_supplier_{rhs.value_attr_supplier_} {
  connectSignals();
}

void KbNodeLinkAttribute::copyExceptSupplier(
    const fto::KbNodeLinkAttribute& other) {
  const KbNodeLinkAttribute* rhs =
      boost::polymorphic_downcast<const KbNodeLinkAttribute*>(&other);
  value_attr_supplier_ = rhs->value_attr_supplier_;
  link_type_ = rhs->link_type_;
}

KbNodeLinkAttribute::~KbNodeLinkAttribute() = default;

void KbNodeLinkAttribute::connectSignals() {
  link_type_.whenLinkUpdated([this]() { linkUpdated(); }, nullptr);
  value_attr_supplier_.whenAttributeChanged([this](IAttribute* attr) {
    (void)attr;
    emitAttributeChanged();
  }, nullptr);
}

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
    : public GenericAttributeSupplier<fto::KbNodeLinkAttribute> {
 public:
  KbNodeLinkAttributeSupplierPrivate(int max_attrs)
      : GenericAttributeSupplier{"", max_attrs} {}

  fto::KbNodeLinkAttribute* createAttribute() const override {
    if (unlikely(attr_factory_)) {
      return attr_factory_->createAttribute();
    } else {
      return new KbNodeLinkAttribute(q_ptr_);
    }
  }

 private:
  KbNodeLinkAttributeSupplier* q_ptr_;
  KbNodeLinkAttributeFactory* attr_factory_{nullptr};

  friend class KbNodeLinkAttributeSupplier;
};

KbNodeLinkAttributeSupplier::KbNodeLinkAttributeSupplier(
    ITreeItemProvider* link_type_item_provider,
    const fto::LinkType* default_proto_link_type, IKbNode* root_kbnode,
    int max_attrs)
    : impl_(utils::make_unique<KbNodeLinkAttributeSupplierPrivate>(max_attrs)),
      link_type_item_provider_(link_type_item_provider),
      default_proto_link_type_(default_proto_link_type),
      root_kbnode_(root_kbnode) {
  impl_->q_ptr_ = this;
}

KbNodeLinkAttributeSupplier::~KbNodeLinkAttributeSupplier() = default;

KbNodeLinkAttributeSupplier::KbNodeLinkAttributeSupplier(
    const KbNodeLinkAttributeSupplier& rhs)
    : impl_{utils::make_unique<KbNodeLinkAttributeSupplierPrivate>(*rhs.impl_)},
      link_type_item_provider_(rhs.link_type_item_provider_),
      default_proto_link_type_(rhs.default_proto_link_type_),
      root_kbnode_(rhs.root_kbnode_) {
  impl_->q_ptr_ = this;
  impl_->cloneAttributes(*rhs.impl_);
}

void KbNodeLinkAttributeSupplier::setAttributeFactory(
    KbNodeLinkAttributeFactory* attr_factory) {
  impl_->attr_factory_ = attr_factory;
}

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
