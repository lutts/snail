// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_UIENGINE_H_
#define INCLUDE_SNAIL_I_UIENGINE_H_

#include "utils/signal_slot.h"

namespace snailcore {

class IUiEngine {
 public:
  virtual ~IUiEngine() = default;

  virtual int run() = 0;
  virtual void quit() = 0;

  SNAIL_SIGSLOT_PURE_VIRTUAL(AboutToQuit, void());
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_UIENGINE_H_
