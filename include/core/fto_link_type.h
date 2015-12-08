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
#include "utils/u8string.h"
#include "utils/signal_slot.h"
#include "test/test_proxy.h"

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

namespace snailcore {

class IAttributeSupplier;

namespace fto {

#ifndef DISABLE_TEST_CODE

#define LinkType_METHODS                                      \
  SNAIL_CONST_INTERFACE0(attributeSuppliers,                  \
                         std::vector<IAttributeSupplier*>()); \
  SNAIL_CONST_INTERFACE0(prototype, const fto::LinkType*());  \
  SNAIL_CONST_INTERFACE0(toString, utils::U8String());        \
  SNAIL_INTERFACE0(clear, void());

class LinkType : public ITreeItem {
 public:
  virtual ~LinkType() = default;

  SNAIL_SIGSLOT_PURE_VIRTUAL(LinkUpdated, void());

  SNAIL_CONST_INTERFACE0(clone, LinkType*());
  virtual LinkType& operator=(LinkType&& rhs) = 0;

  LinkType_METHODS
};

#else  // DISABLE_TEST_CODE

class LinkType : public ITreeItem {};

#endif  // DISABLE_TEST_CODE

}  // namespace fto
}  // namespace snailcore

#undef INTERFACE_DEFINITION_PHASE

#define INTERFACE_TEST_PROXY_PHASE
#include "test/interface.h"  // NOLINT

namespace snailcore {
namespace fto {

class LinkTypeTestProxy {
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(LinkType);
  TEST_PROXY_ENABLE_COPY_CONSTRUCT(LinkType);
  TEST_PROXY_ENABLE_COPY_ASSIGNMENT(LinkType);
  TEST_PROXY_ENABLE_MOVE_CONSTRUCT(LinkType);
  TEST_PROXY_ENABLE_MOVE_ASSIGNMENT(LinkType);

 public:
  utils::U8String name() const { return self_->name(); }

  bool isGroupOnly() const { return self_->isGroupOnly(); }

  SNAIL_SIGSLOT_PROXY(LinkType, LinkUpdated);

  LinkType_METHODS
};

}  // namespace fto
}  // namespace snailcore

#undef INTERFACE_TEST_PROXY_PHASE

#endif  // INCLUDE_CORE_FTO_LINK_TYPE_H_
