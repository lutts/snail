// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_LINK_ATTRIBUTE_H_
#define SRC_CORE_KBNODE_LINK_ATTRIBUTE_H_

#include "include/config.h"
#include "utils/basic_utils.h"
#include "core/generic_attribute_supplier.h"
#include "core/fto_link_type.h"
#include "core/fto_kbnode_attribute.h"
#include "core/fto_kbnode_link_attribute.h"

namespace snailcore {

class IKbNode;

class KbNodeLinkAttributeSupplier
    : public FTO_NAMESPACE::KbNodeLinkAttributeSupplier {
 public:
  KbNodeLinkAttributeSupplier(ITreeItemProvider* link_type_item_provider,
                              const fto::LinkType* default_proto_link_type,
                              IKbNode* root_kbnode, int max_attrs)
      : FTO_NAMESPACE::KbNodeLinkAttributeSupplier("", max_attrs),
        link_type_item_provider_(link_type_item_provider),
        default_proto_link_type_(default_proto_link_type),
        root_kbnode_(root_kbnode) {}

  virtual ~KbNodeLinkAttributeSupplier() = default;

  ITreeItemProvider* getLinkTypeItemProvider() const {
    return link_type_item_provider_;
  }

  const fto::LinkType* getDefaultProtoLinkType() const {
    return default_proto_link_type_;
  }

  IKbNode* getRootKbNode() const { return root_kbnode_; }

  fto::KbNodeLinkAttributeSupplier* clone() const {
    // // TODO(lutts): impl this method
    return nullptr;
  }

 private:
  // TODO(lutts): remove this to allow copy
  SNAIL_DISABLE_COPY(KbNodeLinkAttributeSupplier);

  // GenericAttributeSupplier impls
  IAttribute* createAttribute() override;

  ITreeItemProvider* link_type_item_provider_;
  const fto::LinkType* default_proto_link_type_;
  IKbNode* root_kbnode_;
};

class KbNodeLinkAttribute : public FTO_NAMESPACE::KbNodeLinkAttribute {
 public:
  explicit KbNodeLinkAttribute(
      fto::KbNodeLinkAttributeSupplier* link_attr_supplier);
  virtual ~KbNodeLinkAttribute();

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
  void linkUpdated();
  void initValueAttr();
  void emitAttributeChanged();

  // TODO(lutts): remove this to allow copy
  SNAIL_DISABLE_COPY(KbNodeLinkAttribute);

  fto::KbNodeLinkAttributeSupplier* link_attr_supplier_;
  TEST_PROXY(LinkType) link_type_;

  TEST_PROXY(KbNodeAttributeSupplier) value_attr_supplier_;
  fto::KbNodeAttribute* value_attr_{nullptr};
};

}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_LINK_ATTRIBUTE_H_
