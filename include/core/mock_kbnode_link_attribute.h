// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_H_
#define INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_H_

#include "core/i_kbnode_link_attribute.h"
#include "snail/mock_attribute.h"

namespace snailcore {
namespace tests {

class MockKbNodeLinkAttribute : public IKbNodeLinkAttribute {
 public:
  COMMON_ATTRIBUTE_MOCKS

  MOCK_CONST_METHOD0(supplier, IKbNodeLinkAttributeSupplier*());
  MOCK_METHOD0(valueAttr, IAttribute*());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_H_
