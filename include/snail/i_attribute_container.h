// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ATTRIBUTE_CONTAINER_H_
#define INCLUDE_SNAIL_I_ATTRIBUTE_CONTAINER_H_

#include <vector>
#include "utils/u8string.h"

namespace snailcore {

class IAttribute;

class IAttributeContainer {
 public:
  virtual ~IAttributeContainer() = default;
  virtual utils::U8String getDescription() const = 0;
  virtual std::vector<IAttribute*> getAllowedAttributeList() const = 0;
  virtual IAttribute* getDefaultAllowedAttribute() const = 0;
  // TODO(lutts): do we need to use unique_ptr to transfer the attr ownership?
  virtual void addAttribute(IAttribute* attr) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ATTRIBUTE_CONTAINER_H_
