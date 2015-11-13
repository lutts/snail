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

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

namespace snailcore {

class ITreeItemProvider;
class IKbNode;

namespace fto {

#ifndef DISABLE_TEST_CODE

class KbNodeAttribute;
class LinkType;

#define KbNodeLinkAttributeSupplier_METHODS                             \
  SNAIL_CONST_INTERFACE0(getLinkTypeItemProvider, ITreeItemProvider*()); \
  SNAIL_CONST_INTERFACE0(getDefaultProtoLinkType, const LinkType*());   \
  SNAIL_CONST_INTERFACE0(getRootKbNode, IKbNode*());                    \
  SNAIL_CONST_INTERFACE0(clone, fto::KbNodeLinkAttributeSupplier*());

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
  SNAIL_INTERFACE0(linkType, LinkType*());

class KbNodeLinkAttribute : public IAttribute {
 public:
  virtual ~KbNodeLinkAttribute() = default;

  KbNodeLinkAttribute_METHODS
};

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

#undef INTERFACE_DEFINITION_PHASE

#endif  // INCLUDE_CORE_FTO_KBNODE_LINK_ATTRIBUTE_H_
