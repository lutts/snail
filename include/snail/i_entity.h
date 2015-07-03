// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ENTITY_H_
#define INCLUDE_SNAIL_I_ENTITY_H_

#include "utils/u8string.h"

namespace snailcore {

class IEntity {
 public:
  virtual ~IEntity() = default;

  virtual utils::U8String name() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ENTITY_H_
