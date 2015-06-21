// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include "src/qtui/main_window.h"

#include <QToolBar>
#include <QCloseEvent>
#include <QPushButton>
#include <QDebug>

#include "qtui/i_workspace_view.h"
#include "src/qtui/work_name_input_dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
  createToolbars();

#if 0
  auto widget = workSpaceView_->getWidget();
  if (widget) {
    setCentralWidget(widget);
  }
#endif
}

MainWindow::~MainWindow() {
  // we will destroy the workspace by auto_destroy_view, so remove it from
  // the main layout to avoid unexpected behaviours (eg. relayout after delete)
  setCentralWidget(nullptr);
}

void MainWindow::showView(bool modal) {
  (void)modal;
  show();
}

void MainWindow::setGeometry(int w, int h) {
  resize(w, h);
}

void MainWindow::setWindowTitle2(const utils::U8String& title) {
  setWindowTitle(U8StringToQString(title));
}

void MainWindow::setWorkSpaceView(IWorkSpaceView* workspace_view) {
  QWidget* central_widget = nullptr;

  if (workspace_view) {
    central_widget = workspace_view->getWidget();
  }

  setCentralWidget(central_widget);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (RequestClose())
    event->accept();
  else
    event->ignore();
}

void MainWindow::createToolbars() {
  QToolBar *toolbar;

  toolbar = addToolBar(tr(""));

#if 0
  toolbar->addAction(undoRedoView_->getUndoAction());
  toolbar->addAction(undoRedoView_->getRedoAction());
#endif

  QPushButton *newWorkBtn = new QPushButton(tr("New Work"));
  newWorkBtn->setObjectName(QStringLiteral("newWorkButton"));

  connect(newWorkBtn, &QPushButton::clicked,
            [this]() {
            WorkNameInputDialog workNameDlg(this);
            if (workNameDlg.exec() == QDialog::Accepted) {
              UserClickAddWork(QStringToU8String(workNameDlg.name()));
            }
          });

  toolbar->addWidget(newWorkBtn);
}
