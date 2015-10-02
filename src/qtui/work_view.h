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

class WorkView : public QWidget, public IWorkView {
  //  Q_OBJECT

 public:
  WorkView();
  ~WorkView() = default;

  QWidget* getWidget() const override {
    auto t = const_cast<WorkView*>(this);
    return static_cast<QWidget*>(t);
  }

 private:
  WorkView(const WorkView& other) = delete;
  WorkView& operator=(const WorkView& other) = delete;
};

#endif  // SRC_QTUI_WORK_VIEW_H_
