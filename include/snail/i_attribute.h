// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ATTRIBUTE_H_
#define INCLUDE_SNAIL_I_ATTRIBUTE_H_

#include "utils/u8string.h"

namespace snailcore {

class IAttribute {
 public:
  virtual ~IAttribute() = default;

  virtual utils::U8String displayName() const = 0;
  virtual utils::U8String valueText() const = 0;
  virtual bool isEmpty() const = 0;
  virtual void clear() = 0;

  // TODO(lutts): impl this method in subclasses
  virtual IAttribute* clone() { return nullptr; }
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ATTRIBUTE_H_
