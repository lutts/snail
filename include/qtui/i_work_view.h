// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_WORK_VIEW_H_
#define INCLUDE_QTUI_I_WORK_VIEW_H_

#include "src/qtui/generic_view_base.h"
#include "utils/signal_slot.h"

class IWorkBasicInfoQModel;

class IWorkView : public GenericViewBase {
 public:
  virtual ~IWorkView() = default;

  virtual void setBasicInfoQModel(IWorkBasicInfoQModel* model) = 0;
};


#endif  // INCLUDE_QTUI_I_WORK_VIEW_H_
