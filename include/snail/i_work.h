// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_WORK_H_
#define INCLUDE_SNAIL_I_WORK_H_

#include "utils/u8string.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IWork {
 public:
  virtual ~IWork() = default;

  SNAIL_SIGSLOT2(BasicInfoChanged, void());

  virtual bool set_name(const utils::U8String& name) = 0;
  virtual const utils::U8String& name() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_WORK_H_
