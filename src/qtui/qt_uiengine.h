// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_QT_UIENGINE_H_
#define SRC_QTUI_QT_UIENGINE_H_

#include <QApplication>

#include "snail/i_uiengine.h"
#include "snail/signal_slot_impl.h"

class SnailGenericView;

/**
 * NOTE: must construct a QtUiEngine instance before create any Qt widgets,
 * and must destroy any widgets before destruct the QtUiEngine instance
 */
class QtUiEngine : public QObject, public snailcore::IUiEngine {
  Q_OBJECT

 public:
  QtUiEngine();
  ~QtUiEngine() = default;

  int run(snailcore::SnailGenericView* mainWindow) override;
  void quit() override;

 private slots:
  // slot that get signal when that application is about to quit
  void aboutToQuitApp();

 private:
  int dummy_argc { 0 };
  std::unique_ptr<QApplication> qtApplication;
  snailcore::SnailGenericView* mainWindow_ { nullptr };

 private:
  SNAIL_SIGSLOT_IMPL(AboutToQuit)
};

#endif  // SRC_QTUI_QT_UIENGINE_H_
