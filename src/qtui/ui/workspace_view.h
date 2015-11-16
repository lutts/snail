// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_UI_WORKSPACE_VIEW_H_
#define SRC_QTUI_UI_WORKSPACE_VIEW_H_

#include <QTabWidget>

#include "qtui/ui/i_workspace_view.h"
#include "qtui/ui/i_work_view.h"
#include "utils/signal_slot_impl.h"

class WorkSpaceView : public QTabWidget, public IWorkSpaceView {
 public:
  WorkSpaceView();
  ~WorkSpaceView();

  void addWorkView(IWorkView* work_view,
                   const utils::U8String& work_name) override;
  void removeWorkView(IWorkView* work_view) override;
  void setActiveWorkView(IWorkView* work_view) override;
  void updateWorkViewTitle(IWorkView* work_view,
                           const utils::U8String& new_title) override;

  QWidget* getWidget() override {
    auto t = const_cast<WorkSpaceView*>(this);
    return static_cast<QWidget*>(t);
  }

 private:
  WorkSpaceView(const WorkSpaceView& other) = delete;
  WorkSpaceView& operator=(const WorkSpaceView& other) = delete;

  IWorkView* widgetToWorkView(QWidget* widget) const;

 private:
  SNAIL_SIGSLOT_OVERRIDE_IMPL(UserClickAddWork)
  SNAIL_SIGSLOT_OVERRIDE_IMPL(UserCloseWork)
};

#endif  // SRC_QTUI_UI_WORKSPACE_VIEW_H_
