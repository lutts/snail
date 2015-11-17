// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_ATTRIBUTE_H_
#define SRC_CORE_KBNODE_ATTRIBUTE_H_

#include <algorithm>

#include "include/config.h"
#include "utils/basic_utils.h"
#include "core/generic_attribute_supplier.h"

#include FTO_HEADER(core, kbnode_attribute)

namespace snailcore {

class IKbNode;

class KbNodeAttributeSupplier : public FTO_NAMESPACE::KbNodeAttributeSupplier {
 public:
  KbNodeAttributeSupplier(IKbNode* root_kbnode, int max_attrs);
  virtual ~KbNodeAttributeSupplier();

  IKbNode* getRootKbNode() const;

  // TODO(lutts): impl this method
  KbNodeAttributeSupplier* clone() const { return nullptr; }

 private:
  // GenericAttributeSupplier impls
  IAttribute* createAttribute() override;

  SNAIL_DISABLE_COPY(KbNodeAttributeSupplier);

  IKbNode* root_kbnode_{nullptr};
};

class KbNodeAttribute final : public FTO_NAMESPACE::KbNodeAttribute {
 public:
  explicit KbNodeAttribute(fto::KbNodeAttributeSupplier* attr_supplier);
  virtual ~KbNodeAttribute();

  KbNodeAttribute(const KbNodeAttribute& rhs);
  KbNodeAttribute(KbNodeAttribute&& rhs);
  KbNodeAttribute& operator=(KbNodeAttribute rhs);

  // IAttribute
  utils::U8String displayName() const override;
  utils::U8String valueText() const override;
  bool isEmpty() const override;
  void clear() override;
  void accept(IAttributeVisitor* visitor) override;

  fto::KbNodeAttributeSupplier* supplier() const;
  void setKbNode(IKbNode* kbnode);
  IKbNode* getKbNode() const { return kbnode_; }

  // Test proxy requirement
  KbNodeAttribute* self() { return this; }
  fto::KbNodeAttribute* clone() const { return new KbNodeAttribute(*this); }

  TEST_ONLY_MOVE_ASSIGNMENT(KbNodeAttribute);

 private:
  KbNodeAttribute& swap(KbNodeAttribute& rhs);

  fto::KbNodeAttributeSupplier* attr_supplier_;
  IKbNode* kbnode_{nullptr};
};

}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_ATTRIBUTE_H_
