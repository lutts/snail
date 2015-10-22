// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ATTRIBUTE_SUPPLIER_H_
#define INCLUDE_SNAIL_I_ATTRIBUTE_SUPPLIER_H_

#include <vector>

#include "utils/signal_slot.h"
#include "utils/u8string.h"

namespace snailcore {

class IAttribute;

class IAttributeSupplier {
 public:
  virtual ~IAttributeSupplier() = default;

  SNAIL_SIGSLOT2(AttributeChanged, void(IAttribute* attr));

  virtual utils::U8String name() const = 0;
  virtual int attr_count() const = 0;
  virtual std::vector<IAttribute*> attributes() const = 0;
  virtual int max_attrs() const = 0;

  virtual IAttribute* addAttribute() = 0;
  virtual void removeAttribute(IAttribute* attr) = 0;

  virtual void attributeChanged(IAttribute* attr) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ATTRIBUTE_SUPPLIER_H_
