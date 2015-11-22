// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_H_
#define INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_H_

// region: Mocks

#include "core/fto_kbnode_link_attribute.h"
#include "snail/mock_attribute.h"           // COMMON_ATTRIBUTE_MOCKS
#include "snail/mock_attribute_supplier.h"  // COMMON_ATTR_SUPPLIER_MOCKS
#include "core/fto_kbnode_attribute.h"
#include "core/fto_link_type.h"

#define INTERFACE_MOCK_PHASE
#include "test/interface.h"

namespace snailcore {
namespace tests {

class MockKbNodeLinkAttributeSupplier
    : public fto::KbNodeLinkAttributeSupplier {
 public:
  // MockKbNodeLinkAttributeSupplier() : KbNodeLinkAttributeSupplier("", 0) {}

  COMMON_ATTR_SUPPLIER_MOCKS;

  SNAIL_CONST_INTERFACE0(clone, fto::KbNodeLinkAttributeSupplier*());

  KbNodeLinkAttributeSupplier_METHODS
};

class MockKbNodeLinkAttribute : public fto::KbNodeLinkAttribute {
 public:
  COMMON_ATTRIBUTE_MOCKS
  KbNodeLinkAttribute_METHODS
};

}  // namespace tests
}  // namespace snailcore

#undef INTERFACE_MOCK_PHASE

// endregion: Mocks

#endif  // INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_H_
