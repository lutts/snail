// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_UI_MOCK_MAIN_WINDOW_VIEW_H_
#define INCLUDE_QTUI_UI_MOCK_MAIN_WINDOW_VIEW_H_

#include "qtui/ui/i_main_window_view.h"

class MockMainWindowView : public IMainWindowView {
 public:
  SNAIL_MOCK_SLOT(UserClickAddWork);
  SNAIL_MOCK_SLOT(RequestClose);

  MOCK_METHOD1(showView, bool(bool modal));
  MOCK_METHOD1(setWindowTitle2, void(const utils::U8String& title));
  MOCK_METHOD2(setGeometry, void(int w, int h));

  MOCK_METHOD1(setWorkSpaceView, void(IWorkSpaceView* workspace_view));
};

#endif  // INCLUDE_QTUI_UI_MOCK_MAIN_WINDOW_VIEW_H_
