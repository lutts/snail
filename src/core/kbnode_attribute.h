// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef KBNODE_ATTRIBUTE_H_
#define KBNODE_ATTRIBUTE_H_

#define FTO(x) x

#include "utils/basic_utils.h"
#include "core/generic_attribute.h"
#include "core/kbnode_attribute_supplier.h"
#include "core/i_attribute_visitor.h"

namespace snailcore {

class KbNodeAttribute FTO(: public GenericAttribute) {
 public:
  explicit KbNodeAttribute(KbNodeAttributeSupplier* attr_supplier);
  virtual ~KbNodeAttribute();

  // IAttribute
  utils::U8String valueText() const override;
  bool isEmpty() const override;
  void clear() override;

  void accept(IAttributeVisitor* visitor) override {
    visitor->visit(this);
  }

  // KbNodeAttribute
  KbNodeAttributeSupplier* supplier() const;
  void setKbNode(IKbNode* kbnode);

 private:
  SNAIL_DISABLE_COPY(KbNodeAttribute);

  KbNodeAttributeSupplier* attr_supplier_;
  IKbNode* kbnode_ { nullptr };
};

}  // namespace snailcore

#endif  // KBNODE_ATTRIBUTE_H_
