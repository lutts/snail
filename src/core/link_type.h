// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_LINK_TYPE_H_
#define SRC_CORE_LINK_TYPE_H_

#include <algorithm>
#include <vector>

#include "include/config.h"
#include "snail/i_tree_item.h"
#include "utils/u8string.h"
#include "utils/text/variable_resolver.h"
#include "snail/i_attribute_supplier.h"
#include "utils/text/fto_named_string_formatter.h"
#include "core/fto_link_type.h"

namespace snailcore {

class LinkTypeSignalHelper;

class LinkType : public FTO_NAMESPACE::LinkType,
                 public utils::text::VariableResolver {
 public:
  LinkType(const utils::U8String& name, bool is_group_only);
  LinkType(LinkType&& rhs);
  LinkType(const LinkType& rhs);
  virtual ~LinkType();

  LinkType& operator=(LinkType rhs);

  // ITreeItem impls
  utils::U8String name() const override;
  bool isGroupOnly() const override;

  void setAttributeSuppliers(
      std::vector<std::unique_ptr<IAttributeSupplier>>&& attr_suppliers);
  std::vector<IAttributeSupplier*> attributeSuppliers() const;

  const fto::LinkType* prototype() const;
  utils::U8String toString() const;
  void clear();

  void setLinkPhrase(const utils::U8String& link_phrase);

  // VariableResolver impl
  std::vector<utils::U8String> lookup(const utils::U8String& var_name) override;

  // Test proxy requirment
  LinkType* self() { return this; }
  fto::LinkType* clone() const { return new LinkType(*this); }
  TEST_ONLY_MOVE_ASSIGNMENT(LinkType);

 public:
  SNAIL_SIGSLOT_NONVIRTUAL(LinkUpdated, void());

 private:
  void swap(LinkType& rhs) noexcept;
  const LinkType* getPrototype() const;
  void monitorAttributeSuppliers();
  void resetConnections();

  friend void swap(LinkType& v1, LinkType& v2);

  std::unique_ptr<LinkTypeSignalHelper> signal_helper_;

  utils::U8String name_;
  bool is_group_only_;
  const LinkType* prototype_{nullptr};

  std::vector<std::unique_ptr<IAttributeSupplier>> attr_suppliers_;

  utils::U8String link_phrase_;
  utils::text::TEST_PROXY(NamedStringFormatter) named_string_formatter_;

  std::vector<SignalConnection> connections_;
};

void swap(LinkType& v1, LinkType& v2);

}  // namespace snailcore

#endif  // SRC_CORE_LINK_TYPE_H_
