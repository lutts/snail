// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_WORKSPACE_VIEW_H_
#define MOCK_WORKSPACE_VIEW_H_

#include "qtui/i_workspace_view.h"

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

#endif  // MOCK_WORKSPACE_VIEW_H_
