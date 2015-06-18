// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_WORK_VIEW_H_
#define I_WORK_VIEW_H_

#include "src/qtui/generic_view_base.h"
#include "utils/signal_slot.h"

class IWorkView : public GenericViewBase {
 public:
  virtual ~IWorkView() = default;
};


#endif  // I_WORK_VIEW_H_
