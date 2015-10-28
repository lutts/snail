// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef LINK_TYPE_H_
#define LINK_TYPE_H_

#include "snail/i_tree_item.h"
#include "utils/u8string.h"
#include "snail/i_attribute_supplier.h"

namespace snailcore {

class LinkType : public ITreeItem {
 public:
  LinkType();
  LinkType(const utils::U8String& name,
           bool is_group_only);
  LinkType(const LinkType& rhs);
  virtual ~LinkType();

  utils::U8String name() const override;
  bool isGroupOnly() const override;

  void setAttributeSuppliers(
      std::vector<std::unique_ptr<IAttributeSupplier>>&& attr_suppliers);
  std::vector<IAttributeSupplier*> attributeSuppliers() const;

  LinkType& operator=(LinkType rhs);

 private:
  void swap(LinkType& rhs) noexcept;

  utils::U8String name_;
  bool is_group_only_;

  std::vector<std::unique_ptr<IAttributeSupplier> > attr_suppliers_;
};

}  // namespace snailcore

#endif  // LINK_TYPE_H_
