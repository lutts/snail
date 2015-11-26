// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <algorithm>
#include <vector>

#include "src/core/kbnode_attribute.h"
#include "src/core/generic_attribute_supplier.h"
#include "core/i_kbnode.h"
#include "core/i_attribute_visitor.h"

namespace snailcore {

KbNodeAttribute::KbNodeAttribute(fto::KbNodeAttributeSupplier* attr_supplier)
    : attr_supplier_(attr_supplier) {}

KbNodeAttribute::~KbNodeAttribute() = default;

KbNodeAttribute::KbNodeAttribute(const KbNodeAttribute& rhs)
    : attr_supplier_(rhs.attr_supplier_), kbnode_(rhs.kbnode_) {}

KbNodeAttribute::KbNodeAttribute(KbNodeAttribute&& rhs)
    : attr_supplier_(rhs.attr_supplier_), kbnode_(rhs.kbnode_) {
  rhs.attr_supplier_ = nullptr;
  rhs.kbnode_ = nullptr;
}

KbNodeAttribute& KbNodeAttribute::operator=(KbNodeAttribute rhs) {
  return swap(rhs);
}

KbNodeAttribute& KbNodeAttribute::swap(KbNodeAttribute& rhs) {
  std::swap(attr_supplier_, rhs.attr_supplier_);
  std::swap(kbnode_, rhs.kbnode_);

  return *this;
}

// IAttribute
utils::U8String KbNodeAttribute::displayName() const {
  return attr_supplier_->name();
}

utils::U8String KbNodeAttribute::valueText() const {
  if (isEmpty()) return "";

  return kbnode_->name();
}

bool KbNodeAttribute::isEmpty() const { return kbnode_ == nullptr; }

void KbNodeAttribute::clear() { setKbNode(nullptr); }

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

class KbNodeAttributeSupplierPrivate
    : public GenericAttributeSupplier<KbNodeAttributeSupplier,
                                      KbNodeAttribute> {
 public:
  KbNodeAttributeSupplierPrivate(KbNodeAttributeSupplier* q_ptr,
                                 IKbNode* root_kbnode, int max_attrs)
      : GenericAttributeSupplier{q_ptr, root_kbnode->name(), max_attrs},
        root_kbnode_{root_kbnode} {}

  IKbNode* getRootKbNode() const { return root_kbnode_; }

 private:
  IKbNode* root_kbnode_;
};

KbNodeAttributeSupplier::KbNodeAttributeSupplier(IKbNode* root_kbnode,
                                                 int max_attrs)
    : impl_(utils::make_unique<KbNodeAttributeSupplierPrivate>(
          this, root_kbnode, max_attrs)) {}

KbNodeAttributeSupplier::~KbNodeAttributeSupplier() = default;

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
