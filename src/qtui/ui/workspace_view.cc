// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/ui/workspace_view.h"
#include "utils/u8string.h"

WorkSpaceView::WorkSpaceView() {
  setObjectName(QStringLiteral("WorkSpaceView"));
  setTabsClosable(true);
  setMovable(true);

  connect(this, &QTabWidget::tabCloseRequested, [this](int index) {
    auto w = widget(index);
    UserCloseWork(widgetToWorkView(w));
  });
}

WorkSpaceView::~WorkSpaceView() = default;

void WorkSpaceView::addWorkView(IWorkView* work_view,
                                const utils::U8String& work_name) {
  auto page = work_view->getWidget();
  if (page) addTab(page, U8StringToQString(work_name));
}

void WorkSpaceView::removeWorkView(IWorkView* work_view) {
  auto widget = work_view->getWidget();
  if (widget) removeTab(indexOf(widget));
}

void WorkSpaceView::setActiveWorkView(IWorkView* work_view) {
  auto widget = work_view->getWidget();
  if (widget) setCurrentWidget(widget);
}

void WorkSpaceView::updateWorkViewTitle(IWorkView* work_view,
                                        const utils::U8String& new_title) {
  int index = indexOf(work_view->getWidget());
  if (index >= 0) setTabText(index, U8StringToQString(new_title));
}

IWorkView* WorkSpaceView::widgetToWorkView(QWidget* widget) const {
  return dynamic_cast<IWorkView*>(widget);
}
