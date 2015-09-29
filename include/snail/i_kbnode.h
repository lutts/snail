// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_KBNODE_H_
#define INCLUDE_SNAIL_I_KBNODE_H_

#include "utils/u8string.h"
#include "snail/kb_global.h"

namespace snailcore {

class IKbNode {
 public:
  virtual ~IKbNode() = default;

  virtual KbNodeIdType id() const = 0;
  virtual utils::U8String name() const = 0;
  virtual bool isCategory() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_KBNODE_H_
