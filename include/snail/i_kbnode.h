// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_H_
#define I_KBNODE_H_

#include "utils/u8string.h"

namespace snailcore {

class IKbNode {
 public:
  virtual ~IKbNode() = default;

  virtual utils::U8String name() const = 0;
  virtual bool isCategory() const = 0;
};

}  // namespace snailcore

#endif  // I_KBNODE_H_
