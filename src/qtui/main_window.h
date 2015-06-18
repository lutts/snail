// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_MAIN_WINDOW_H_
#define SRC_QTUI_MAIN_WINDOW_H_

#include <QMainWindow>

#include <memory>  // shared_ptr

#include "qtui/i_main_window_view.h"
// #include "qtui/i_undo_redo_view.h"
#include "utils/signal_slot_impl.h"
#include "utils/and_slot_bool_result_combiner.h"

// #include "qtui/i_workspace_view.h"

class MainWindow : public QMainWindow, public IMainWindowView {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void showView(bool modal) override;

  void setWindowTitle2(const utils::U8String& title) override;
  void setGeometry(int w, int h) override;

  void setWorkSpaceView(IWorkSpaceView* workspace_view) override;

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  void createToolbars();

 private:
  SNAIL_SIGSLOT_COMBINER_IMPL(RequestClose, and_slot_bool_result_combiner)
};

#endif  // SRC_QTUI_MAIN_WINDOW_H_
