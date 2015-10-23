// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_LINK_ATTRIBUTE_H_
#define INCLUDE_CORE_I_KBNODE_LINK_ATTRIBUTE_H_


#include "snail/i_attribute.h"
#include "core/generic_attribute_supplier.h"

#ifndef DISABLE_TEST_CODE

namespace snailcore {

class ITreeItemProvider;

namespace fto {

class KbNodeLinkAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeLinkAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeLinkAttributeSupplier() = default;

  virtual ITreeItemProvider* getLinkTypeProvider() const = 0;
};

class KbNodeLinkAttribute : public IAttribute {
 public:
  virtual ~KbNodeLinkAttribute() = default;

  virtual KbNodeLinkAttributeSupplier* supplier() const = 0;
  virtual IAttribute* valueAttr() = 0;
};

}  // namespace fto
}  // namespace snailcore

#else  // DISABLE_TEST_CODE

#include "include/config.h"

namespace snailcore {
FTO_BEGIN_NAMESPACE

class KbNodeLinkAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeLinkAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeLinkAttributeSupplier() = default;;
};

class KbNodeLinkAttribute : public IAttribute { };

FTO_END_NAMESPACE
}  // namespace snailcore

#endif  // DISABLE_TEST_CODE
#endif  // INCLUDE_CORE_I_KBNODE_LINK_ATTRIBUTE_H_
