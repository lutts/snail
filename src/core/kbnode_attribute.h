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

class KbNodeAttribute;

class KbNodeAttributeFactory {
 public:
  virtual ~KbNodeAttributeFactory() = default;

  virtual fto::KbNodeAttribute* createAttribute() const = 0;
};

class KbNodeAttributeSupplier : public FTO_NAMESPACE::KbNodeAttributeSupplier {
 public:
  KbNodeAttributeSupplier(IKbNode* root_kbnode, int max_attrs);
  KbNodeAttributeSupplier(const KbNodeAttributeSupplier& rhs);
  // TODO(lutts): impl KbNodeAttributeSupplier copy assignment
  KbNodeAttributeSupplier& operator=(const KbNodeAttributeSupplier& rhs);
  virtual ~KbNodeAttributeSupplier();

  KbNodeAttributeSupplier* clone() const {
    return new KbNodeAttributeSupplier(*this);
  }

  utils::U8String name() const override;
  int attr_count() const override;
  std::vector<IAttribute*> attributes() const override;
  int max_attrs() const override;

  IAttribute* addAttribute() override;
  void removeAttribute(IAttribute* attr) override;

  void attributeChanged(IAttribute* attr) override;

  IKbNode* getRootKbNode() const;

  void setAttributeFactory(KbNodeAttributeFactory* attr_factory);

  // region: Test proxy requirement
  KbNodeAttributeSupplier* self() { return this; }
  TEST_ONLY_COPY_ASSIGNMENT(KbNodeAttributeSupplier);
  // endregion: Test proxy requirement

 public:
  SNAIL_SIGSLOT_OVERRIDE(AttributeChanged);

 private:
  std::unique_ptr<KbNodeAttributeSupplierPrivate> impl_;
  friend class KbNodeAttributeSupplierPrivate;
};

struct KbNodeAttributeData {
  IKbNode* kbnode_{nullptr};

  utils::U8String valueText() const;

  bool isEmpty() const { return kbnode_ == nullptr; }

  bool setKbNode(IKbNode* kbnode) {
    if (kbnode_ != kbnode) {
      kbnode_ = kbnode;
      return true;
    }

    return false;
  }
  void clear() { kbnode_ = nullptr; }
};

class KbNodeAttribute final : public FTO_NAMESPACE::KbNodeAttribute {
 public:
  explicit KbNodeAttribute(fto::KbNodeAttributeSupplier* attr_supplier);
  virtual ~KbNodeAttribute();

  KbNodeAttribute(const KbNodeAttribute& rhs);
  KbNodeAttribute(KbNodeAttribute&& rhs);
  KbNodeAttribute& operator=(KbNodeAttribute rhs);

  void copyData(const fto::KbNodeAttribute& other);

  // IAttribute
  utils::U8String displayName() const override;
  utils::U8String valueText() const override;
  bool isEmpty() const override;
  void clear() override;
  void accept(IAttributeVisitor* visitor) override;

  fto::KbNodeAttributeSupplier* supplier() const;
  void setKbNode(IKbNode* kbnode);

  // Test proxy requirement
  KbNodeAttribute* self() { return this; }
  fto::KbNodeAttribute* clone() const { return new KbNodeAttribute(*this); }

  TEST_ONLY_MOVE_ASSIGNMENT(KbNodeAttribute);

 private:
  KbNodeAttribute& swap(KbNodeAttribute& rhs);

  fto::KbNodeAttributeSupplier* attr_supplier_;
  KbNodeAttributeData data_;
};

}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_ATTRIBUTE_H_
