// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_WORK_VIEW_H_
#define INCLUDE_QTUI_MOCK_WORK_VIEW_H_

#include "qtui/i_work_view.h"

class MockWorkView : public IWorkView {
 public:
  SNAIL_MOCK_SLOT(UserClickAddAttribute);

  MOCK_METHOD1(setBasicInfoQModel, void(IWorkBasicInfoQModel* model));
};

#endif  // INCLUDE_QTUI_MOCK_WORK_VIEW_H_
