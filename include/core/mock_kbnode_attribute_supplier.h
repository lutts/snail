// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_ATTRIBUTE_SUPPLIER_H_
#define MOCK_KBNODE_ATTRIBUTE_SUPPLIER_H_

#include "core/i_kbnode_attribute_supplier.h"

namespace snailcore {
namespace tests {

class MockKbNodeAttributeSupplier : public IKbNodeAttributeSupplier {
 public:
  MockKbNodeAttributeSupplier()
      : IKbNodeAttributeSupplier("", 0) { }
  // GenericAttributeSupplier mocks
  MOCK_METHOD0(createAttribute, IAttribute*());

  // Self
  MOCK_CONST_METHOD0(getRootKbNodeId, KbNodeIdType());
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_ATTRIBUTE_SUPPLIER_H_
