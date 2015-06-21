// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]


#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>

#include "src/qtui/work_name_input_dialog.h"

WorkNameInputDialog::WorkNameInputDialog(QWidget *parent)
    : QDialog(parent) {
  setObjectName(QStringLiteral("newWorkNameDialog"));
  setModal(true);

  QVBoxLayout *verticalLayout = new QVBoxLayout(this);
  verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
  verticalLayout->setContentsMargins(0, 0, 0, 0);

  QHBoxLayout *horizontalLayout = new QHBoxLayout();
  horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

  QLabel *label = new QLabel(this);
  horizontalLayout->addWidget(label);

  workName = new QLineEdit(this);
  workName->setObjectName(QStringLiteral("workName"));
  horizontalLayout->addWidget(workName);

  verticalLayout->addLayout(horizontalLayout);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
  buttonBox->setObjectName(QStringLiteral("buttonBox"));
  buttonBox->setOrientation(Qt::Horizontal);
  buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
  buttonBox->setCenterButtons(true);
  QPushButton *okBtn = buttonBox->button(QDialogButtonBox::Ok);
  okBtn->setEnabled(false);

  verticalLayout->addWidget(buttonBox);

  connect(workName, &QLineEdit::textChanged,
          [okBtn](const QString & text) {
            okBtn->setEnabled(!text.isEmpty());
          });

  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

const QString WorkNameInputDialog::name() const {
  return workName->text();
}
void WorkNameInputDialog::setName(const QString& defaultName) {
  workName->setText(defaultName);
}
