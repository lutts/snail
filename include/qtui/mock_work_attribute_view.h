// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_WORK_ATTRIBUTE_VIEW_H_
#define MOCK_WORK_ATTRIBUTE_VIEW_H_

#include "qtui/i_work_attribute_view.h"

class MockWorkAttributeView : public IWorkAttributeView {
 public:
  SNAIL_MOCK_SLOT(EditModeButtonClicked);
  SNAIL_MOCK_SLOT(DoneButtonClicked);

  MOCK_METHOD0(beginReLayoutAttributes, void());
  MOCK_METHOD0(endReLayoutAttributes, void());

  MOCK_METHOD5(addLabel, void(const utils::U8String& label,
                              int row, int column,
                              int row_span, int column_span));
  MOCK_METHOD5(addAttribute, void(IAttributeView* attr_view,
                                  int row, int column,
                                  int row_span, int column_span));
  MOCK_METHOD5(addEraseCommand, void(utils::Command* command,
                                     int row, int column,
                                     int row_span, int column_span));
  MOCK_METHOD5(addPopupEditorCommand, void(utils::Command* command,
                                           int row, int column,
                                           int row_span, int column_span));
  MOCK_METHOD5(addAddAttributeCommand, void(utils::Command* command,
                                            int row, int column,
                                            int row_span, int column_span));
};

#endif  // MOCK_WORK_ATTRIBUTE_VIEW_H_
