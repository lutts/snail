// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_GENERIC_VIEW_BASE_H_
#define INCLUDE_QTUI_GENERIC_VIEW_BASE_H_

#include "pfmvp/i_pf_view.h"

class QWidget;

class GenericViewBase : public pfmvp::IPfView {
 public:
  virtual ~GenericViewBase() = default;

  virtual QWidget* getWidget() { return nullptr; }
};

#endif  // INCLUDE_QTUI_GENERIC_VIEW_BASE_H_
