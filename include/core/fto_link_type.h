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

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

#define LinkType_METHODS                        \
  SNAIL_CONST_INTERFACE0(attributeSuppliers,    \
                         std::vector<IAttributeSupplier*>());

class LinkType : public ITreeItem {
 public:
  virtual ~LinkType() = default;

  SNAIL_CONST_INTERFACE0(clone, fto::LinkType*());
  LinkType_METHODS
};

#undef INTERFACE_DEFINITION_PHASE

#define INTERFACE_TEST_PROXY_PHASE
#include "test/interface.h"

class LinkTypeTestProxy : public LinkType {
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(LinkType);

 public:
  utils::U8String name() const override {
    return real_obj_->name();
  }

  bool isGroupOnly() const override {
    return real_obj_->isGroupOnly();
  }

  LinkType_METHODS
};

#undef INTERFACE_TEST_PROXY_PHASE

#else  // DISABLE_TEST_CODE

class LinkType : public ITreeItem { };

#endif  // DISABLE_TEST_CODE

}  // namespace fto

}  // namespace snailcore

#endif  // INCLUDE_CORE_FTO_LINK_TYPE_H_
