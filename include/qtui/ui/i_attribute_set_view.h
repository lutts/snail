// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_UI_I_ATTRIBUTE_SET_VIEW_H_
#define INCLUDE_QTUI_UI_I_ATTRIBUTE_SET_VIEW_H_

#include "qtui/ui/generic_view_base.h"
#include "utils/signal_slot.h"

class IAttributeSetView : public GenericViewBase {
 public:
  virtual ~IAttributeSetView() = default;

  SNAIL_SIGSLOT_PURE_VIRTUAL(UserSwitchMode, void());

  virtual void setSwitchModelButtonEnabled(bool enabled) = 0;
  virtual void switchToEditMode() = 0;
  virtual void switchToDisplayMode() = 0;
};


#endif  // INCLUDE_QTUI_UI_I_ATTRIBUTE_SET_VIEW_H_
