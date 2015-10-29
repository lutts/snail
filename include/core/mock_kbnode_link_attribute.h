// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_H_
#define INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_H_

#include "core/fto_kbnode_link_attribute.h"
#include "snail/mock_attribute.h"
#include "core/fto_kbnode_attribute.h"
#include "core/fto_link_type.h"

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
  MOCK_CONST_METHOD0(getLinkTypeItemProvider, ITreeItemProvider*());
  MOCK_CONST_METHOD0(getDefaultProtoLinkType, const fto::LinkType*());
};

class MockKbNodeLinkAttribute : public fto::KbNodeLinkAttribute {
 public:
  COMMON_ATTRIBUTE_MOCKS

  MOCK_CONST_METHOD0(supplier, fto::KbNodeLinkAttributeSupplier*());
  MOCK_METHOD0(valueAttr, fto::KbNodeAttribute*());
  MOCK_METHOD1(setValueAttr, void(const fto::KbNodeAttribute& value_attr));

  MOCK_METHOD1(setProtoLinkType, void(const fto::LinkType* proto_link_type));
  MOCK_CONST_METHOD0(protoLinkType, const fto::LinkType*());
  MOCK_METHOD0(linkType, fto::LinkType*());
  MOCK_METHOD1(setLinkType, void(const fto::LinkType& link_type));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_H_
