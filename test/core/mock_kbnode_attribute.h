// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_

#include "core/kbnode_attribute.h"

namespace snailcore {
namespace tests {

class MockKbNodeAttribute : public KbNodeAttribute {
 public:
  MockKbNodeAttribute() : KbNodeAttribute(nullptr) { }
  // IAttribute mocks
  MOCK_CONST_METHOD0(valueText, utils::U8String());
  MOCK_CONST_METHOD0(isEmpty, bool());
  MOCK_METHOD0(clear, void());

  // Self
  MOCK_CONST_METHOD0(supplier, KbNodeAttributeSupplier*());
  MOCK_METHOD1(setKbNode, void(IKbNode* kbnode));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_
