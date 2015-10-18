// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_LINK_ATTRIBUTE_H_
#define MOCK_KBNODE_LINK_ATTRIBUTE_H_

#include "core/i_kbnode_link_attribute.h"

namespace snailcore {
namespace tests {

class MockKbNodeLinkAttribute : public IKbNodeLinkAttribute {
 public:
  MockKbNodeLinkAttribute() : IKbNodeLinkAttribute(nullptr) { }
  // IAttribute mocks
  MOCK_CONST_METHOD0(valueText, utils::U8String());
  MOCK_CONST_METHOD0(isEmpty, bool());
  MOCK_METHOD0(clear, void());

  // self
  MOCK_CONST_METHOD0(supplier, IKbNodeLinkAttributeSupplier*());
  MOCK_METHOD0(valueAttr, IAttribute*());
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_LINK_ATTRIBUTE_H_
