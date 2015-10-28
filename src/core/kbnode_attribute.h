// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_ATTRIBUTE_H_
#define SRC_CORE_KBNODE_ATTRIBUTE_H_

#include "include/config.h"
#include "utils/basic_utils.h"
#include "core/generic_attribute_supplier.h"

#include FTO_HEADER(core, kbnode_attribute)

namespace snailcore {

class IKbNode;

class KbNodeAttributeSupplier : public FTO_INTERFACE(KbNodeAttributeSupplier) {
 public:
  KbNodeAttributeSupplier(IKbNode* root_kbnode, int max_attrs);
  virtual ~KbNodeAttributeSupplier();

  // IAttributeSupplier impls
  IAttributeSupplier* clone() const override { return nullptr; }  // TODO(lutts): impl this

  // GenericAttributeSupplier impls
  IAttribute* createAttribute() override;

  IKbNode* getRootKbNode() const;

 private:
  SNAIL_DISABLE_COPY(KbNodeAttributeSupplier);

  IKbNode* root_kbnode_ { nullptr };
};

class KbNodeAttribute : public FTO_INTERFACE(KbNodeAttribute) {
 public:
  KbNodeAttribute() = default;
  explicit KbNodeAttribute(fto::KbNodeAttributeSupplier* attr_supplier);
  virtual ~KbNodeAttribute();

  // IAttribute
  utils::U8String displayName() const override;
  utils::U8String valueText() const override;
  bool isEmpty() const override;
  void clear() override;
  void accept(IAttributeVisitor* visitor) override;

  fto::KbNodeAttributeSupplier* supplier() const;
  void setKbNode(IKbNode* kbnode);

  KbNodeAttribute& operator=(const KbNodeAttribute& rhs) {
    attr_supplier_ = rhs.attr_supplier_;
    kbnode_ = rhs.kbnode_;

    return *this;
  }

 private:
  fto::KbNodeAttributeSupplier* attr_supplier_ { nullptr };
  IKbNode* kbnode_ { nullptr };
};

}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_ATTRIBUTE_H_
