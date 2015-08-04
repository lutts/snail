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
  MOCK_METHOD1(updateLabel, void(snailcore::UpdateAttrLabelData label_data));
  MOCK_METHOD1(endLayout, void(bool remove_triads));
};

class MockWorkAttributeView : public IWorkAttributeView {
 public:
  SNAIL_MOCK_SLOT(EditModeButtonClicked);
  SNAIL_MOCK_SLOT(DoneButtonClicked);

  MOCK_METHOD1(setEditMode, void(bool edit_mode));
};

#endif  // INCLUDE_QTUI_MOCK_WORK_ATTRIBUTE_VIEW_H_
