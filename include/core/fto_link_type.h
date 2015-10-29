// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_FTO_LINK_TYPE_H_
#define INCLUDE_CORE_FTO_LINK_TYPE_H_

#include <vector>

#include "snail/i_tree_item.h"
#include "test/test_proxy.h"

namespace snailcore {

class IAttributeSupplier;

namespace fto {

#ifndef DISABLE_TEST_CODE

class LinkType : public ITreeItem {
 public:
  virtual ~LinkType() = default;

  virtual LinkType* clone() const = 0;
  virtual std::vector<IAttributeSupplier*> attributeSuppliers() const = 0;
};

class LinkTypeTestProxy : public LinkType {
  TEST_PROXY_WITH_DEFAULT_CONSTRUCTOR(LinkType);

 public:
  utils::U8String name() const override {
    return real_obj_->name();
  }

  bool isGroupOnly() const override {
    return real_obj_->isGroupOnly();
  }

  std::vector<IAttributeSupplier*> attributeSuppliers() const override {
    return real_obj_->attributeSuppliers();
  }
};

#else  // DISABLE_TEST_CODE

class LinkType : public ITreeItem { };

#endif  // DISABLE_TEST_CODE

}  // namespace fto

}  // namespace snailcore

#endif  // INCLUDE_CORE_FTO_LINK_TYPE_H_
