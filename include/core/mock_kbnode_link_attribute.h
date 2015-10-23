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

class MockKbNodeLinkAttributeSupplier
    : public fto::KbNodeLinkAttributeSupplier {
 public:
  MockKbNodeLinkAttributeSupplier()
      : fto::KbNodeLinkAttributeSupplier("", 0) { }

  // IAttributeSupplier mocks
  MOCK_METHOD1(attributeChanged, void(IAttribute* attr));

  // GenericAttributeSupplier mocks
  MOCK_METHOD0(createAttribute, IAttribute*());

  // Self
  MOCK_CONST_METHOD0(getLinkTypeProvider, ITreeItemProvider*());
};

class MockKbNodeLinkAttribute : public fto::KbNodeLinkAttribute {
 public:
  COMMON_ATTRIBUTE_MOCKS

  MOCK_CONST_METHOD0(supplier, fto::KbNodeLinkAttributeSupplier*());
  MOCK_METHOD0(valueAttr, IAttribute*());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_H_
