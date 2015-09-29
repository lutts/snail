// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_SUPPLIER_H_
#define INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_SUPPLIER_H_

#include "core/i_kbnode_attribute_supplier.h"

namespace snailcore {
namespace tests {

class MockKbNodeAttributeSupplier : public IKbNodeAttributeSupplier {
 public:
  MockKbNodeAttributeSupplier()
      : IKbNodeAttributeSupplier("", 0) { }
  // IAttributeSupplier mocks
  MOCK_METHOD1(attributeChanged, void(IAttribute* attr));

  // GenericAttributeSupplier mocks
  MOCK_METHOD0(createAttribute, IAttribute*());

  // Self
  MOCK_CONST_METHOD0(getRootKbNode, IKbNode*());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_SUPPLIER_H_
