// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_ATTRIBUTE_EDITOR_VIEW_H_
#define INCLUDE_QTUI_MOCK_ATTRIBUTE_EDITOR_VIEW_H_

#include "qtui/i_attribute_editor_view.h"

class MockAttributeEditorView : public IAttributeEditorView {
 public:
  MOCK_METHOD0(getWidget, QWidget*());
};

#endif  // INCLUDE_QTUI_MOCK_ATTRIBUTE_EDITOR_VIEW_H_
