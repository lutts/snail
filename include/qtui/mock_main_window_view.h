// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_MAIN_WINDOW_VIEW_H_
#define INCLUDE_QTUI_MOCK_MAIN_WINDOW_VIEW_H_

#include "qtui/i_main_window_view.h"

class MockMainWindowView : public IMainWindowView {
 public:
  SNAIL_MOCK_SLOT(RequestClose);

  MOCK_METHOD1(showView, void(bool modal));
  MOCK_METHOD1(setWindowTitle2, void(const utils::U8String& title));
  MOCK_METHOD2(setGeometry, void(int w, int h));
};


#endif  // INCLUDE_QTUI_MOCK_MAIN_WINDOW_VIEW_H_
