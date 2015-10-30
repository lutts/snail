// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_FTO_KBNODE_LINK_ATTRIBUTE_H_
#define INCLUDE_CORE_FTO_KBNODE_LINK_ATTRIBUTE_H_

#include "include/config.h"
#include "snail/i_attribute.h"
#include "core/generic_attribute_supplier.h"

namespace snailcore {

class ITreeItemProvider;

namespace fto {

#ifndef DISABLE_TEST_CODE

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

class KbNodeAttribute;
class LinkType;

#define KbNodeLinkAttributeSupplier_METHODS                             \
  SNAIL_CONST_INTERFACE0(getLinkTypeItemProvider, ITreeItemProvider*()); \
  SNAIL_CONST_INTERFACE0(getDefaultProtoLinkType, const LinkType*());

class KbNodeLinkAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeLinkAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeLinkAttributeSupplier() = default;

  KbNodeLinkAttributeSupplier_METHODS
};

#define KbNodeLinkAttribute_METHODS                                     \
  SNAIL_CONST_INTERFACE0(supplier, KbNodeLinkAttributeSupplier*());     \
  SNAIL_INTERFACE0(valueAttr, KbNodeAttribute*());                      \
  SNAIL_INTERFACE1(setValueAttr, void(const KbNodeAttribute& value_attr)); \
                                                                        \
  SNAIL_INTERFACE1(setProtoLinkType, void(const LinkType* proto_link_type)); \
  SNAIL_CONST_INTERFACE0(protoLinkType, const LinkType*());             \
  SNAIL_INTERFACE0(linkType, LinkType*());                              \
  SNAIL_INTERFACE1(setLinkType, void(const LinkType& link_type));        \

class KbNodeLinkAttribute : public IAttribute {
 public:
  virtual ~KbNodeLinkAttribute() = default;

  KbNodeLinkAttribute_METHODS
};

#undef INTERFACE_DEFINITION_PHASE

#else  // DISABLE_TEST_CODE

class KbNodeLinkAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeLinkAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeLinkAttributeSupplier() = default;;
};

class KbNodeLinkAttribute : public IAttribute { };

#endif  // DISABLE_TEST_CODE
}  // namespace fto
}  // namespace snailcore

#endif  // INCLUDE_CORE_FTO_KBNODE_LINK_ATTRIBUTE_H_
