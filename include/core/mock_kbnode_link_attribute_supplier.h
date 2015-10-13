// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_LINK_ATTRIBUTE_SUPPLIER_H_
#define MOCK_KBNODE_LINK_ATTRIBUTE_SUPPLIER_H_

namespace snailcore {
namespace tests {

class MockKbNodeLinkAttributeSupplier : public IKbNodeLinkAttributeSupplier {
 public:
  MockKbNodeLinkAttributeSupplier()
      : IKbNodeLinkAttributeSupplier("", 0) { }

  // IAttributeSupplier mocks
  MOCK_METHOD1(attributeChanged, void(IAttribute* attr));

  // GenericAttributeSupplier mocks
  MOCK_METHOD0(createAttribute, IAttribute*());
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_LINK_ATTRIBUTE_SUPPLIER_H_
