// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_ATTRIBUTE_H_
#define SRC_CORE_ATTRIBUTE_H_

#include "utils/basic_utils.h"
#include "core/i_kbnode_attribute.h"

namespace snailcore {

class KbNodeAttribute : public IKbNodeAttribute {
 public:
  explicit KbNodeAttribute(IKbNodeAttributeSupplier* attr_supplier);
  virtual ~KbNodeAttribute();

  // IAttribute
  utils::U8String displayName() const override;
  utils::U8String valueText() const override;
  bool isEmpty() const override;
  void clear() override;

  void accept(IAttributeVisitor* visitor) override;

  // IKbNodeAttribute
  IKbNodeAttributeSupplier* supplier() const override;
  void setKbNode(IKbNode* kbnode) override;

 private:
  SNAIL_DISABLE_COPY(KbNodeAttribute);

  IKbNodeAttributeSupplier* attr_supplier_;
  IKbNode* kbnode_ { nullptr };
};

class KbNodeAttributeSupplier : public IKbNodeAttributeSupplier {
 public:
  KbNodeAttributeSupplier(IKbNode* root_kbnode, int max_attrs);
  virtual ~KbNodeAttributeSupplier();

  // GenericAttributeSupplier impls
  IAttribute* createAttribute() override;

  // IKbNodeAttributeSupplier impls
  IKbNode* getRootKbNode() const override;

 private:
  SNAIL_DISABLE_COPY(KbNodeAttributeSupplier);

  IKbNode* root_kbnode_ { nullptr };
};



}  // namespace snailcore

#endif  // SRC_CORE_ATTRIBUTE_H_
