// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_WORKSPACE_VIEW_H_
#define I_WORKSPACE_VIEW_H_

#include "src/qtui/generic_view_base.h"
#include "utils/signal_slot.h"
#include "utils/u8string.h"

class IWorkView;

class IWorkSpaceView : public GenericViewBase {
 public:
  virtual ~IWorkSpaceView() = default;

  SNAIL_SIGSLOT2(UserClickAddWork, void(const utils::U8String& work_name));
  SNAIL_SIGSLOT2(UserCloseWork, void(IWorkView* work_view));

  virtual void addWorkView(IWorkView* work_view,
                           const utils::U8String& work_name) = 0;
  virtual void setActiveWorkView(IWorkView* work_view) = 0;
  virtual void removeWorkView(IWorkView* work_view) = 0;
  virtual void updateWorkViewTitle(IWorkView* work_view,
                                   const utils::U8String& new_title) = 0;
};


#endif  // I_WORKSPACE_VIEW_H_
