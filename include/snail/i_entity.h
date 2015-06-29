// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_ENTITY_H_
#define I_ENTITY_H_

#include "utils/u8string.h"

namespace snailcore {

class IEntity {
 public:
  virtual ~IEntity() = default;

  virtual utils::U8String name() const = 0;
};

}  // namespace snailcore

#endif  // I_ENTITY_H_
