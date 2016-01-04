// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <boost/polymorphic_cast.hpp>

#include <algorithm>
#include <vector>

#include "src/core/kbnode_attribute.h"
#include "src/core/generic_attribute_supplier.h"
#include "core/i_kbnode.h"
#include "core/i_attribute_visitor.h"
#include "utils/compiler.h"

namespace snailcore {

bool KbNodeAttributeData::setKbNode(IKbNode* kbnode) {
  if (kbnode_ != kbnode) {
    if (kbnode_) kbnode_->decRef();

    kbnode_ = kbnode;

    if (kbnode_) kbnode_->incRef();
    return true;
  }

  return false;
}

KbNodeAttribute::KbNodeAttribute(fto::KbNodeAttributeSupplier* attr_supplier)
    : attr_supplier_{attr_supplier} {}

KbNodeAttribute::~KbNodeAttribute() = default;

KbNodeAttribute::KbNodeAttribute(const KbNodeAttribute& rhs)
    : attr_supplier_{rhs.attr_supplier_}, data_{rhs.data_} {}

KbNodeAttribute::KbNodeAttribute(KbNodeAttribute&& rhs)
    : attr_supplier_{rhs.attr_supplier_}, data_{rhs.data_} {
  rhs.attr_supplier_ = nullptr;
  rhs.data_.clear();
}

KbNodeAttribute& KbNodeAttribute::operator=(KbNodeAttribute rhs) {
  return swap(rhs);
}

KbNodeAttribute& KbNodeAttribute::swap(KbNodeAttribute& rhs) {
  std::swap(attr_supplier_, rhs.attr_supplier_);
  std::swap(data_, rhs.data_);

  attr_supplier_->attributeChanged(this);

  return *this;
}

void KbNodeAttribute::copyData(const fto::KbNodeAttribute& other) {
  const KbNodeAttribute* rhs =
      boost::polymorphic_downcast<const KbNodeAttribute*>(&other);
  data_ = rhs->data_;
}

// IAttribute
utils::U8String KbNodeAttribute::displayName() const {
  return attr_supplier_->name();
}

utils::U8String KbNodeAttributeData::valueText() const {
  return kbnode_->name();
}

utils::U8String KbNodeAttribute::valueText() const {
  if (isEmpty()) return "";

  return data_.valueText();
}

bool KbNodeAttribute::isEmpty() const { return data_.isEmpty(); }

void KbNodeAttribute::clear() { setKbNode(nullptr); }

void KbNodeAttribute::accept(IAttributeVisitor* visitor) {
  visitor->visit(this);
}

// IKbNodeAttribute
fto::KbNodeAttributeSupplier* KbNodeAttribute::supplier() const {
  return attr_supplier_;
}

void KbNodeAttribute::setKbNode(IKbNode* kbnode) {
  if (data_.setKbNode(kbnode)) {
    attr_supplier_->attributeChanged(this);
  }
}

//////////////// KbNodeAttributeSupplier ////////////////
class KbNodeAttributeSupplierPrivate
    : public GenericAttributeSupplier<fto::KbNodeAttribute> {
 public:
  KbNodeAttributeSupplierPrivate(IKbNode* root_kbnode, int max_attrs)
      : GenericAttributeSupplier{root_kbnode->name(), max_attrs},
        root_kbnode_{root_kbnode} {}

  KbNodeAttributeSupplierPrivate(const KbNodeAttributeSupplierPrivate& rhs)
      : GenericAttributeSupplier{rhs},
        root_kbnode_{rhs.root_kbnode_},
        attr_factory_{rhs.attr_factory_} {}

  KbNodeAttributeSupplierPrivate& operator=(
      const KbNodeAttributeSupplierPrivate& rhs) {
    GenericAttributeSupplier::operator=(rhs);
    root_kbnode_ = rhs.root_kbnode_;
    attr_factory_ = rhs.attr_factory_;
    // q_ptr_ is not changed
    return *this;
  }

  IKbNode* getRootKbNode() const { return root_kbnode_; }

  fto::KbNodeAttribute* createAttribute() const override {
    if (unlikely(attr_factory_)) {
      return attr_factory_->createAttribute();
    } else {
      return new KbNodeAttribute(q_ptr_);
    }
  }

 private:
  KbNodeAttributeSupplier* q_ptr_;
  IKbNode* root_kbnode_;
  KbNodeAttributeFactory* attr_factory_{nullptr};

  friend class KbNodeAttributeSupplier;
};

KbNodeAttributeSupplier::KbNodeAttributeSupplier(IKbNode* root_kbnode,
                                                 int max_attrs)
    : impl_(utils::make_unique<KbNodeAttributeSupplierPrivate>(root_kbnode,
                                                               max_attrs)) {
  impl_->q_ptr_ = this;
}

KbNodeAttributeSupplier::KbNodeAttributeSupplier(
    const KbNodeAttributeSupplier& rhs)
    : impl_(utils::make_unique<KbNodeAttributeSupplierPrivate>(*rhs.impl_)) {
  impl_->q_ptr_ = this;
  impl_->cloneAttributes(*rhs.impl_);
}

KbNodeAttributeSupplier& KbNodeAttributeSupplier::operator=(
    const KbNodeAttributeSupplier& rhs) {
  *impl_ = *rhs.impl_;
  impl_->clearAttributes();
  impl_->cloneAttributes(*rhs.impl_);

  return *this;
}

KbNodeAttributeSupplier::~KbNodeAttributeSupplier() = default;

void KbNodeAttributeSupplier::setAttributeFactory(
    KbNodeAttributeFactory* attr_factory) {
  impl_->attr_factory_ = attr_factory;
}

SNAIL_SIGSLOT_DELEGATE(KbNodeAttributeSupplier, AttributeChanged, impl_);

utils::U8String KbNodeAttributeSupplier::name() const { return impl_->name(); }
int KbNodeAttributeSupplier::attr_count() const { return impl_->attr_count(); }
std::vector<IAttribute*> KbNodeAttributeSupplier::attributes() const {
  return impl_->attributes();
}
int KbNodeAttributeSupplier::max_attrs() const { return impl_->max_attrs(); }
IAttribute* KbNodeAttributeSupplier::addAttribute() {
  return impl_->addAttribute();
}
void KbNodeAttributeSupplier::removeAttribute(IAttribute* attr) {
  impl_->removeAttribute(attr);
}
void KbNodeAttributeSupplier::attributeChanged(IAttribute* attr) {
  impl_->attributeChanged(attr);
}
IKbNode* KbNodeAttributeSupplier::getRootKbNode() const {
  return impl_->getRootKbNode();
}

}  // namespace snailcore
