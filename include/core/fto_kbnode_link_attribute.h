// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_FTO_KBNODE_LINK_ATTRIBUTE_H_
#define INCLUDE_CORE_FTO_KBNODE_LINK_ATTRIBUTE_H_


#include "snail/i_attribute.h"
#include "core/generic_attribute_supplier.h"

#ifndef DISABLE_TEST_CODE

namespace snailcore {

class ITreeItemProvider;

namespace fto {

class KbNodeAttribute;
class LinkType;

class KbNodeLinkAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeLinkAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeLinkAttributeSupplier() = default;

  virtual ITreeItemProvider* getLinkTypeItemProvider() const = 0;
  virtual const LinkType* getDefaultProtoLinkType() const = 0;
};

class KbNodeLinkAttribute : public IAttribute {
 public:
  virtual ~KbNodeLinkAttribute() = default;

  virtual KbNodeLinkAttributeSupplier* supplier() const = 0;
  virtual KbNodeAttribute* valueAttr() = 0;
  virtual void setValueAttr(const KbNodeAttribute& value_attr) = 0;
  virtual const LinkType* protoLinkType() const = 0;
  virtual void setProtoLinkType(const LinkType* proto_link_type) = 0;
  virtual LinkType* linkType() = 0;
  virtual void setLinkType(const LinkType& link_type) = 0;
};

}  // namespace fto
}  // namespace snailcore

#else  // DISABLE_TEST_CODE

#include "include/config.h"

namespace snailcore {
namespace fto {

class KbNodeLinkAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeLinkAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeLinkAttributeSupplier() = default;;
};

class KbNodeLinkAttribute : public IAttribute { };

}  // namespace fto
}  // namespace snailcore

#endif  // DISABLE_TEST_CODE
#endif  // INCLUDE_CORE_FTO_KBNODE_LINK_ATTRIBUTE_H_
