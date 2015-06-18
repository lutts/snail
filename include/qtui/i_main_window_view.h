// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_MAIN_WINDOW_VIEW_H_
#define INCLUDE_QTUI_I_MAIN_WINDOW_VIEW_H_

#include "src/qtui/generic_view_base.h"
#include "utils/signal_slot.h"
#include "utils/u8string.h"

class IWorkSpaceView;

class IMainWindowView : public GenericViewBase {
 public:
  virtual ~IMainWindowView() = default;

  SNAIL_SIGSLOT2(RequestClose, bool());

  virtual void setWindowTitle2(const utils::U8String& title) = 0;
  virtual void setGeometry(int w, int h) = 0;

  virtual void setWorkSpaceView(IWorkSpaceView* workspace_view) = 0;
};

#endif  // INCLUDE_QTUI_I_MAIN_WINDOW_VIEW_H_
