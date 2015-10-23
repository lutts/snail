// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_

#include "core/i_kbnode_attribute.h"
#include "snail/mock_attribute.h"

namespace snailcore {
namespace tests {

class MockKbNodeAttribute : public IKbNodeAttribute {
 public:
  COMMON_ATTRIBUTE_MOCKS

  MOCK_CONST_METHOD0(supplier, IKbNodeAttributeSupplier*());
  MOCK_METHOD1(setKbNode, void(IKbNode* kbnode));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_
