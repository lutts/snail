// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_UI_I_WORKSPACE_VIEW_H_
#define INCLUDE_QTUI_UI_I_WORKSPACE_VIEW_H_

#include "qtui/ui/generic_view_base.h"
#include "utils/signal_slot.h"
#include "utils/u8string.h"

class IWorkView;

class IWorkSpaceView : public GenericViewBase {
 public:
  virtual ~IWorkSpaceView() = default;

  SNAIL_SIGSLOT_PURE_VIRTUAL(UserClickAddWork,
                             void(const utils::U8String& work_name));
  SNAIL_SIGSLOT_PURE_VIRTUAL(UserCloseWork, void(IWorkView* work_view));

  virtual void addWorkView(IWorkView* work_view,
                           const utils::U8String& work_name) = 0;
  virtual void setActiveWorkView(IWorkView* work_view) = 0;
  virtual void removeWorkView(IWorkView* work_view) = 0;
  virtual void updateWorkViewTitle(IWorkView* work_view,
                                   const utils::U8String& new_title) = 0;
};

#endif  // INCLUDE_QTUI_UI_I_WORKSPACE_VIEW_H_
