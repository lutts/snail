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

KbNodeLinkAttributeData::KbNodeLinkAttributeData(
    const fto::LinkType& proto_link_type, IKbNode* root_kbnode)
    : link_type_{proto_link_type}, value_attr_supplier_{root_kbnode, 1} {
  value_attr_supplier_.addAttribute();
}

utils::U8String KbNodeLinkAttributeData::displayName() const {
  auto link_name = link_type_.toString();
  if (link_name.empty())
    return value_attr_supplier_.name();
  else
    return link_name;
}

fto::KbNodeAttribute* KbNodeLinkAttributeData::value_attr() const {
  IAttribute* attr{nullptr};

  if (value_attr_supplier_.attr_count() == 0) {
    auto that = const_cast<KbNodeLinkAttributeData*>(this);
    attr = that->value_attr_supplier_.addAttribute();
  } else {
    attr = value_attr_supplier_.attributes()[0];
  }

  return static_cast<fto::KbNodeAttribute*>(attr);
}

utils::U8String KbNodeLinkAttributeData::valueText() const {
  auto attr = value_attr();
  if (attr)
    return attr->valueText();
  else
    return "";
}

bool KbNodeLinkAttributeData::isEmpty() const {
  auto attr = value_attr();
  if (attr)
    return attr->isEmpty();
  else
    return true;
}

void KbNodeLinkAttributeData::clear() {
  auto attr = value_attr();
  if (attr) attr->clear();
  link_type_.clear();
}

void KbNodeLinkAttributeData::connectSignals(KbNodeLinkAttribute* owner) {
  link_type_.whenLinkUpdated([owner]() { owner->emitAttributeChanged(); },
                             nullptr);
  value_attr_supplier_.whenAttributeChanged([owner](IAttribute* attr) {
    (void)attr;
    owner->emitAttributeChanged();
  }, nullptr);
}

KbNodeLinkAttribute::KbNodeLinkAttribute(
    fto::KbNodeLinkAttributeSupplier* link_attr_supplier)
    : link_attr_supplier_(link_attr_supplier),
      data_{*link_attr_supplier_->getDefaultProtoLinkType(),
            link_attr_supplier_->getRootKbNode()} {
  data_.connectSignals(this);
}

KbNodeLinkAttribute::KbNodeLinkAttribute(const KbNodeLinkAttribute& rhs)
    : link_attr_supplier_{rhs.link_attr_supplier_}, data_{rhs.data_} {
  data_.connectSignals(this);
}

void KbNodeLinkAttribute::copyData(const fto::KbNodeLinkAttribute& other) {
  const KbNodeLinkAttribute* rhs =
      boost::polymorphic_downcast<const KbNodeLinkAttribute*>(&other);
  data_ = rhs->data_;
}

KbNodeLinkAttribute::~KbNodeLinkAttribute() = default;

utils::U8String KbNodeLinkAttribute::displayName() const {
  return data_.displayName();
}

utils::U8String KbNodeLinkAttribute::valueText() const {
  return data_.valueText();
}

void KbNodeLinkAttribute::emitAttributeChanged() {
  link_attr_supplier_->attributeChanged(this);
}

bool KbNodeLinkAttribute::isEmpty() const { return data_.isEmpty(); }
void KbNodeLinkAttribute::clear() { data_.clear(); }
void KbNodeLinkAttribute::accept(IAttributeVisitor* visitor) {
  visitor->visit(this);
}

fto::KbNodeLinkAttributeSupplier* KbNodeLinkAttribute::supplier() const {
  return link_attr_supplier_;
}

fto::KbNodeAttribute* KbNodeLinkAttribute::valueAttr() {
  return data_.value_attr();
}

fto::LinkType* KbNodeLinkAttribute::linkType() { return data_.link_type(); }

//////////////// KbNodeLinkAttributeSupplier impls ////////////////

class KbNodeLinkAttributeSupplierPrivate
    : public GenericAttributeSupplier<fto::KbNodeLinkAttribute> {
 public:
  explicit KbNodeLinkAttributeSupplierPrivate(int max_attrs)
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
