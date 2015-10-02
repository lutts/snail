// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_WORK_VIEW_H_
#define SRC_QTUI_WORK_VIEW_H_

#include <QWidget>

#include "qtui/i_work_view.h"
#include "utils/signal_slot_impl.h"

namespace QSint {
class ActionGroup;
}  // namespace QSint

class WorkView : public QWidget, public IWorkView {
  //  Q_OBJECT

 public:
  WorkView();
  ~WorkView() = default;

  QWidget* getWidget() override {
    auto t = const_cast<WorkView*>(this);
    return static_cast<QWidget*>(t);
  }

 private:
  WorkView(const WorkView& other) = delete;
  WorkView& operator=(const WorkView& other) = delete;

  QSint::ActionGroup* name_desc_group_ { nullptr };
  // TODO(lutts): this bool var is not need in production code
  bool name_desc_group_editing_ { false };

  QSint::ActionGroup* parts_group_ { nullptr };
  QSint::ActionGroup* scores_group_ { nullptr };
  QSint::ActionGroup* recordings_group_ { nullptr };
  QSint::ActionGroup* related_work_group_ { nullptr };
};

#endif  // SRC_QTUI_WORK_VIEW_H_
