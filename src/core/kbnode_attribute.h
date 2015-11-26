// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_ATTRIBUTE_H_
#define SRC_CORE_KBNODE_ATTRIBUTE_H_

#include <algorithm>
#include <memory>
#include <vector>

#include "include/config.h"
#include "utils/basic_utils.h"
#include "core/fto_kbnode_attribute.h"

namespace snailcore {

class IKbNode;

class KbNodeAttributeSupplierPrivate;

class KbNodeAttributeSupplier : public FTO_NAMESPACE::KbNodeAttributeSupplier {
 public:
  KbNodeAttributeSupplier(IKbNode* root_kbnode, int max_attrs);
  virtual ~KbNodeAttributeSupplier();

  utils::U8String name() const override;
  int attr_count() const override;
  std::vector<IAttribute*> attributes() const override;
  int max_attrs() const override;

  IAttribute* addAttribute() override;
  void removeAttribute(IAttribute* attr) override;

  void attributeChanged(IAttribute* attr) override;

  // TODO(lutts): KbNodeAttributeSupplier clone impl
  KbNodeAttributeSupplier* clone() const override { return nullptr; }

  IKbNode* getRootKbNode() const;

 public:
  SNAIL_SIGSLOT_OVERRIDE(AttributeChanged);

 private:
  SNAIL_DISABLE_COPY(KbNodeAttributeSupplier);

  std::unique_ptr<KbNodeAttributeSupplierPrivate> impl_;
  friend class KbNodeAttributeSupplierPrivate;
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
