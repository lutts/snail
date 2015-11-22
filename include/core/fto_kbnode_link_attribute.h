// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_FTO_KBNODE_LINK_ATTRIBUTE_H_
#define INCLUDE_CORE_FTO_KBNODE_LINK_ATTRIBUTE_H_

// region: shared with impl

#include "include/config.h"
#include "snail/i_attribute.h"
#include "snail/i_attribute_supplier.h"

namespace snailcore {

class ITreeItemProvider;
class IKbNode;

FTO_BEGIN_NAMESPACE
class KbNodeAttribute;
class LinkType;
FTO_END_NAMESPACE

}  // namespace snailcore

// endregion: shared

// region: Interface

#ifndef DISABLE_TEST_CODE

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

namespace snailcore {
namespace fto {

#define KbNodeLinkAttributeSupplier_METHODS                                \
  SNAIL_CONST_INTERFACE0(getLinkTypeItemProvider, ITreeItemProvider*());   \
  SNAIL_CONST_INTERFACE0(getDefaultProtoLinkType, const fto::LinkType*()); \
  SNAIL_CONST_INTERFACE0(getRootKbNode, IKbNode*());

class KbNodeLinkAttributeSupplier : public IAttributeSupplier {
 public:
  virtual ~KbNodeLinkAttributeSupplier() = default;

  SNAIL_CONST_INTERFACE0(clone, fto::KbNodeLinkAttributeSupplier*());

  KbNodeLinkAttributeSupplier_METHODS
};

#define KbNodeLinkAttribute_METHODS                                      \
  SNAIL_CONST_INTERFACE0(supplier, fto::KbNodeLinkAttributeSupplier*()); \
  SNAIL_INTERFACE0(valueAttr, fto::KbNodeAttribute*());                  \
  SNAIL_INTERFACE0(linkType, fto::LinkType*());

class KbNodeLinkAttribute : public IAttribute {
 public:
  virtual ~KbNodeLinkAttribute() = default;

  KbNodeLinkAttribute_METHODS
};

}  // namespace fto
}  // namespace snailcore

#undef INTERFACE_DEFINITION_PHASE

#else  // DISABLE_TEST_CODE

namespace snailcore {
namespace fto {

class KbNodeLinkAttributeSupplier : public IAttributeSupplier {
 public:
  virtual ~KbNodeLinkAttributeSupplier() = default;
};

class KbNodeLinkAttribute : public IAttribute {};

}  // namespace fto
}  // namespace snailcore

#endif  // DISABLE_TEST_CODE
// endregion: Interface

#endif  // INCLUDE_CORE_FTO_KBNODE_LINK_ATTRIBUTE_H_
