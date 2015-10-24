// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CORE_QT_UIENGINE_H_
#define SRC_QTUI_CORE_QT_UIENGINE_H_

#include <QApplication>

#include "snail/i_uiengine.h"
#include "utils/signal_slot_impl.h"

/**
 * NOTE: must construct a QtUiEngine instance before create any Qt widgets,
 * and must destroy any widgets before destruct the QtUiEngine instance
 */
class QtUiEngine : public QObject, public snailcore::IUiEngine {
  Q_OBJECT

 public:
  QtUiEngine();
  ~QtUiEngine() = default;

  int run() override;
  void quit() override;

 private slots:
  // slot that get signal when that application is about to quit
  void aboutToQuitApp();

 private:
  int dummy_argc { 0 };
  std::unique_ptr<QApplication> qtApplication;

 private:
  SNAIL_SIGSLOT_OVERRIDE(AboutToQuit)
};

#endif  // SRC_QTUI_CORE_QT_UIENGINE_H_
