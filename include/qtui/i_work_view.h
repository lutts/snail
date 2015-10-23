// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_WORK_VIEW_H_
#define INCLUDE_QTUI_I_WORK_VIEW_H_

#include "qtui/generic_view_base.h"
#include "utils/signal_slot.h"

class QString;
class IAttributeSetView;

class IWorkView : public GenericViewBase {
 public:
  virtual ~IWorkView() = default;

  SNAIL_SIGSLOT(UserSetWorkName, void(const QString& new_name));

  virtual void setWorkName(const QString& work_name) = 0;

  // sub views
  virtual void setWorkAttrSetView(
      IAttributeSetView* attr_set_view) = 0;
};


#endif  // INCLUDE_QTUI_I_WORK_VIEW_H_
