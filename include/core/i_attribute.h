// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_ATTRIBUTE_H_
#define INCLUDE_CORE_I_ATTRIBUTE_H_

#include "utils/u8string.h"

namespace snailcore {

class IAttribute {
 public:
  virtual ~IAttribute() = default;

  virtual utils::U8String displayName() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_ATTRIBUTE_H_
