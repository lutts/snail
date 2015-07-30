// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_WORK_ATTRIBUTE_VIEW_H_
#define INCLUDE_QTUI_MOCK_WORK_ATTRIBUTE_VIEW_H_

#include "qtui/i_work_attribute_view.h"

class MockAttributeLayout : public IAttributeLayout {
 public:
  MOCK_METHOD1(beginLayout, void(int total_block_count));
  MOCK_METHOD1(layoutAttributeGroupDisplayBlock,
               void*(snailcore::AttributeGroupDisplayBlock attr_group_block));
  MOCK_METHOD1(layoutAttributeDisplayBlock,
               void*(AttributeViewDisplayBlock attr_view_block));
  MOCK_METHOD0(endLayout, void());
};

class MockWorkAttributeView : public IWorkAttributeView {
 public:
  SNAIL_MOCK_SLOT(EditModeButtonClicked);
  SNAIL_MOCK_SLOT(DoneButtonClicked);
};

#endif  // INCLUDE_QTUI_MOCK_WORK_ATTRIBUTE_VIEW_H_
