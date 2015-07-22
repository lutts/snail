// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_ATTRIBUTE_VIEW_H_
#define INCLUDE_QTUI_I_ATTRIBUTE_VIEW_H_

#include "src/qtui/generic_view_base.h"
#include "utils/signal_slot.h"

class IAttributeView : public GenericViewBase {
 public:
  virtual ~IAttributeView() = default;
};


#endif  // INCLUDE_QTUI_I_ATTRIBUTE_VIEW_H_
