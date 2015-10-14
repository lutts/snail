// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_UI_ATTRIBUTE_SET_VIEW_H_
#define SRC_QTUI_UI_ATTRIBUTE_SET_VIEW_H_

#include <QObject>

#include "qtui/i_attribute_set_view.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

namespace QSint {
class ActionGroup;
}  // namespace QSint

class AttributeSetViewForWorkView : public IAttributeSetView
                                  , public QObject {
 public:
  AttributeSetViewForWorkView();
  virtual ~AttributeSetViewForWorkView() = default;

  QWidget* getWidget() override;
  QWidget* getAttributeSetContainer() {
    return attr_set_container_;
  }

  void switchToDisplayMode() override;
  void switchToEditMode() override;
  void setSwitchModelButtonEnabled(bool enabled) override;

  QSint::ActionGroup* action_group_;
  QWidget* attr_set_container_;

 private:
  SNAIL_DISABLE_COPY(AttributeSetViewForWorkView);

  SNAIL_SIGSLOT_IMPL(UserSwitchMode);
};


#endif  // SRC_QTUI_UI_ATTRIBUTE_SET_VIEW_H_
