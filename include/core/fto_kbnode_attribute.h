// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_

#include "snail/i_attribute.h"
#include "core/generic_attribute_supplier.h"

#ifndef DISABLE_TEST_CODE

namespace snailcore {

class IKbNode;

namespace fto {

class KbNodeAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeAttributeSupplier() = default;

  virtual IKbNode* getRootKbNode() const = 0;
};

class KbNodeAttribute : public IAttribute {
 public:
  virtual ~KbNodeAttribute() = default;

  virtual KbNodeAttributeSupplier* supplier() const = 0;
  virtual void setKbNode(IKbNode* kbnode) = 0;
};

}  // namespace fto
}  // namespace snailcore

#else  // DISABLE_TEST_CODE

#include "include/config.h"

namespace snailcore {
namespace fto {

class KbNodeAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeAttributeSupplier() = default;
};

class KbNodeAttribute : public IAttribute { };

}  // namespace fto
}  // namespace snailcore

#endif  // DISABLE_TEST_CODE
#endif  // INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_
