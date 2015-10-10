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
  SNAIL_MOCK_SLOT(UserSetWorkName);

  MOCK_METHOD1(setWorkName, void(const QString& work_name));

  MOCK_METHOD1(setWorkAttrSetView,
               void(IAttributeSetView* attr_set_view));
};

#endif  // INCLUDE_QTUI_MOCK_WORK_VIEW_H_
