// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_

#include "core/fto_kbnode_attribute.h"
#include "snail/mock_attribute.h"

#define INTERFACE_MOCK_PHASE
#include "test/interface.h"

namespace snailcore {
namespace tests {

class MockKbNodeAttribute : public fto::KbNodeAttribute {
 public:
  COMMON_ATTRIBUTE_MOCKS

  SNAIL_CONST_INTERFACE0(clone, fto::KbNodeAttribute*());
  KbNodeAttribute_METHODS
};

class MockKbNodeAttributeSupplier : public fto::KbNodeAttributeSupplier {
 public:
  MockKbNodeAttributeSupplier()
      : fto::KbNodeAttributeSupplier("", 0) { }
  // IAttributeSupplier mocks
  MOCK_CONST_METHOD0(name, utils::U8String());
  MOCK_METHOD1(attributeChanged, void(IAttribute* attr));

  // GenericAttributeSupplier mocks
  MOCK_METHOD0(createAttribute, IAttribute*());

  // Self
  MOCK_CONST_METHOD0(getRootKbNode, IKbNode*());
};

}  // namespace tests
}  // namespace snailcore

#undef INTERFACE_MOCK_PHASE

#endif  // INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_
