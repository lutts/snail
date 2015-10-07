// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_UI_WORK_NAME_INPUT_DIALOG_H_
#define SRC_QTUI_UI_WORK_NAME_INPUT_DIALOG_H_

#include <QDialog>

class QLineEdit;

class WorkNameInputDialog : public QDialog {
 public:
  explicit WorkNameInputDialog(QWidget *parent = 0);

  const QString name() const;
  void setName(const QString& defaultName);

 private:
  QLineEdit *workName;
};

#endif  // SRC_QTUI_UI_WORK_NAME_INPUT_DIALOG_H_
