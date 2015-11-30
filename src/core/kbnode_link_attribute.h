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

class KbNodeLinkAttributeSupplierPrivate;

class KbNodeLinkAttributeSupplier
    : public FTO_NAMESPACE::KbNodeLinkAttributeSupplier {
 public:
  KbNodeLinkAttributeSupplier(ITreeItemProvider* link_type_item_provider,
                              const fto::LinkType* default_proto_link_type,
                              IKbNode* root_kbnode, int max_attrs);

  virtual ~KbNodeLinkAttributeSupplier();

  // IAttributeSupplier impls
  utils::U8String name() const override;
  int attr_count() const override;
  std::vector<IAttribute*> attributes() const override;
  int max_attrs() const override;

  IAttribute* addAttribute() override;
  void removeAttribute(IAttribute* attr) override;

  void attributeChanged(IAttribute* attr) override;

  // TODO(lutts): KbNodeLinkAttributeSupplier clone impl
  KbNodeLinkAttributeSupplier* clone() const override { return nullptr; }

  // self impls
  ITreeItemProvider* getLinkTypeItemProvider() const {
    return link_type_item_provider_;
  }

  const fto::LinkType* getDefaultProtoLinkType() const {
    return default_proto_link_type_;
  }

  IKbNode* getRootKbNode() const { return root_kbnode_; }

 public:
  SNAIL_SIGSLOT_OVERRIDE(AttributeChanged);

 private:
  SNAIL_DISABLE_COPY(KbNodeLinkAttributeSupplier);

  std::unique_ptr<KbNodeLinkAttributeSupplierPrivate> impl_;

  ITreeItemProvider* link_type_item_provider_;
  const fto::LinkType* default_proto_link_type_;
  IKbNode* root_kbnode_;
};

class KbNodeLinkAttribute : public FTO_NAMESPACE::KbNodeLinkAttribute {
 public:
  explicit KbNodeLinkAttribute(
      fto::KbNodeLinkAttributeSupplier* link_attr_supplier);
  virtual ~KbNodeLinkAttribute();

  KbNodeLinkAttribute(const KbNodeLinkAttribute& rhs);

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
  void connectSignals();
  void linkUpdated();
  void initValueAttr();
  void emitAttributeChanged();

  fto::KbNodeLinkAttributeSupplier* link_attr_supplier_;
  TEST_PROXY(LinkType) link_type_;

  TEST_PROXY(KbNodeAttributeSupplier) value_attr_supplier_;
  fto::KbNodeAttribute* value_attr_{nullptr};
};

}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_LINK_ATTRIBUTE_H_
