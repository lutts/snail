// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_ATTRIBUTE_CONTAINER_H_
#define INCLUDE_CORE_I_ATTRIBUTE_CONTAINER_H_

#include <vector>

namespace snailcore {

class IAttributeSupplier;

class IAttributeContainer {
 public:
  virtual ~IAttributeContainer() = default;

  virtual std::vector<IAttributeSupplier*> attr_suppliers() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_ATTRIBUTE_CONTAINER_H_
