// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_WORKSPACE_VIEW_H_
#define INCLUDE_QTUI_MOCK_WORKSPACE_VIEW_H_

#include "qtui/ui/i_workspace_view.h"

class MockWorkSpaceView : public IWorkSpaceView {
 public:
  SNAIL_MOCK_SLOT(UserClickAddWork);
  SNAIL_MOCK_SLOT(UserCloseWork);

  MOCK_METHOD2(addWorkView, void(IWorkView*, const utils::U8String&));
  MOCK_METHOD1(setActiveWorkView, void(IWorkView*));
  MOCK_METHOD1(removeWorkView, void(IWorkView* work_view));
  MOCK_METHOD2(updateWorkViewTitle, void(IWorkView* work_view,
                                         const utils::U8String& new_title));
};

#endif  // INCLUDE_QTUI_MOCK_WORKSPACE_VIEW_H_
