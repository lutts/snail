// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_LINK_ATTRIBUTE_H_
#define SRC_CORE_KBNODE_LINK_ATTRIBUTE_H_

#include <vector>

#include "include/config.h"
#include "utils/basic_utils.h"
#include "core/fto_link_type.h"
#include "core/fto_kbnode_attribute.h"
#include "core/fto_kbnode_link_attribute.h"

namespace snailcore {

class IKbNode;
class ITreeItemProvider;

class KbNodeLinkAttributeFactory {
 public:
  virtual ~KbNodeLinkAttributeFactory() = default;

  virtual fto::KbNodeLinkAttribute* createAttribute() const = 0;
};

class KbNodeLinkAttributeSupplierPrivate;

class KbNodeLinkAttributeSupplier
    : public FTO_NAMESPACE::KbNodeLinkAttributeSupplier {
 public:
  KbNodeLinkAttributeSupplier(ITreeItemProvider* link_type_item_provider,
                              const fto::LinkType* default_proto_link_type,
                              IKbNode* root_kbnode, int max_attrs);
  KbNodeLinkAttributeSupplier(const KbNodeLinkAttributeSupplier& rhs);

  virtual ~KbNodeLinkAttributeSupplier();

  KbNodeLinkAttributeSupplier* clone() const override {
    return new KbNodeLinkAttributeSupplier(*this);
  }

  // IAttributeSupplier impls
  utils::U8String name() const override;
  int attr_count() const override;
  std::vector<IAttribute*> attributes() const override;
  int max_attrs() const override;

  IAttribute* addAttribute() override;
  void removeAttribute(IAttribute* attr) override;

  void attributeChanged(IAttribute* attr) override;

  // self impls
  ITreeItemProvider* getLinkTypeItemProvider() const {
    return link_type_item_provider_;
  }

  const fto::LinkType* getDefaultProtoLinkType() const {
    return default_proto_link_type_;
  }

  IKbNode* getRootKbNode() const { return root_kbnode_; }

  void setAttributeFactory(KbNodeLinkAttributeFactory* attr_factory);

 public:
  SNAIL_SIGSLOT_OVERRIDE(AttributeChanged);

 private:
  std::unique_ptr<KbNodeLinkAttributeSupplierPrivate> impl_;

  ITreeItemProvider* link_type_item_provider_;
  const fto::LinkType* default_proto_link_type_;
  IKbNode* root_kbnode_;
};

class KbNodeLinkAttribute;

class KbNodeLinkAttributeData {
 public:
  KbNodeLinkAttributeData(const fto::LinkType& proto_link_type,
                          IKbNode* root_kbnode);

  void connectSignals(KbNodeLinkAttribute* owner);

  fto::KbNodeAttribute* value_attr() const;
  fto::LinkType* link_type() const { return link_type_.self(); }
  utils::U8String displayName() const;
  utils::U8String valueText() const;
  bool isEmpty() const;
  void clear();

 private:
  void connectSignals();

  TEST_PROXY(LinkType) link_type_;
  TEST_PROXY(KbNodeAttributeSupplier) value_attr_supplier_;
  fto::KbNodeAttribute* value_attr_{nullptr};
};

class KbNodeLinkAttribute : public FTO_NAMESPACE::KbNodeLinkAttribute {
 public:
  explicit KbNodeLinkAttribute(
      fto::KbNodeLinkAttributeSupplier* link_attr_supplier);
  virtual ~KbNodeLinkAttribute();

  KbNodeLinkAttribute(const KbNodeLinkAttribute& rhs);

  void copyData(const fto::KbNodeLinkAttribute& other);

  // IAttribute
  utils::U8String displayName() const override;
  utils::U8String valueText() const override;
  bool isEmpty() const override;
  void clear() override;
  void accept(IAttributeVisitor* visitor) override;

  fto::KbNodeLinkAttributeSupplier* supplier() const;
  fto::KbNodeAttribute* valueAttr();
  fto::LinkType* linkType();

 private:
  void emitAttributeChanged();

  fto::KbNodeLinkAttributeSupplier* link_attr_supplier_;
  KbNodeLinkAttributeData data_;

  friend class KbNodeLinkAttributeData;
};

}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_LINK_ATTRIBUTE_H_
