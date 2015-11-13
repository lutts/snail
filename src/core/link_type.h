// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef LINK_TYPE_H_
#define LINK_TYPE_H_

#include "include/config.h"
#include "snail/i_tree_item.h"
#include "utils/u8string.h"
#include "utils/signal_slot_impl.h"
#include "utils/text/variable_resolver.h"
#include "snail/i_attribute_supplier.h"
#include "utils/text/fto_named_string_formatter.h"

#include FTO_HEADER(core, link_type)

namespace snailcore {

class LinkType : public FTO_NAMESPACE::LinkType
               , public utils::text::VariableResolver {
 public:
  LinkType(const utils::U8String& name,
           bool is_group_only);
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
  fto::LinkType* clone() const {
    return new LinkType(*this);
  }
  TEST_ONLY_MOVE_ASSIGNMENT(LinkType);

 private:
  SNAIL_SIGSLOT_IMPL(LinkUpdated, void());

  LinkType& swap(LinkType& rhs) noexcept;
  const LinkType* getPrototype() const;

  utils::U8String name_;
  bool is_group_only_;
  const LinkType* prototype_ { nullptr };

  std::vector<std::unique_ptr<IAttributeSupplier> > attr_suppliers_;

  utils::U8String link_phrase_;
  utils::text::TEST_PROXY(NamedStringFormatter) named_string_formatter_;
};

}  // namespace snailcore

#endif  // LINK_TYPE_H_
