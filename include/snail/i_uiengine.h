// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_UIENGINE_H_
#define INCLUDE_SNAIL_I_UIENGINE_H_

#include "snail/signal_slot.h"

namespace snailcore {

class SnailGenericView;

class IUiEngine {
 public:
  virtual ~IUiEngine() = default;

  virtual int run(SnailGenericView* mainWindow) = 0;
  virtual void quit() = 0;

  SNAIL_SIGSLOT2(AboutToQuit, void());
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_UIENGINE_H_
