// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_UI_I_ATTRIBUTE_EDITOR_VIEW_H_
#define INCLUDE_QTUI_UI_I_ATTRIBUTE_EDITOR_VIEW_H_

#include "qtui/ui/generic_view_base.h"
#include "utils/signal_slot.h"

class IAttributeEditorView : public GenericViewBase {
 public:
  virtual ~IAttributeEditorView() = default;
};


#endif  // INCLUDE_QTUI_UI_I_ATTRIBUTE_EDITOR_VIEW_H_
