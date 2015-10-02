// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/work_view.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>

#include <QPushButton>
#include <QLabel>
#include <QDebug>

#include "qsint/include/QSint"

WorkView::WorkView() {
  QVBoxLayout *verticalLayout = new QVBoxLayout(this);

  QSint::ActionPanel* action_panel = new QSint::ActionPanel(this);

  QSint::ActionGroup *group1 = action_panel->createGroup(QPixmap(QStringLiteral(":images/filesave.png")), QStringLiteral("Expandable Group"));
  group1->setHasEditButton(true);
  group1->setHeaderTextEditable(true);

  group1->addWidget(new QPushButton(QStringLiteral("Just a button"), this));
  group1->addWidget(new QPushButton(QStringLiteral("Just another button"), this));

  connect(group1, &QSint::ActionGroup::editButtonClicked,
          []() {
            qDebug() << "edit button clicked";
          });

  connect(group1, &QSint::ActionGroup::headerTextChanged,
          [](const QString& text) {
            qDebug() << "header text changed, new text: " << text;
          });

  QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  //  sizePolicy.setHorizontalStretch(0);
  //  sizePolicy.setVerticalStretch(0);
  //  sizePolicy.setHeightForWidth(action_panel->sizePolicy().hasHeightForWidth());
  action_panel->setSizePolicy(sizePolicy);

  action_panel->addStretch();

  verticalLayout->addWidget(action_panel);

  auto label = new QLabel(QStringLiteral("<img src=':images/filesave.png'>Hello!"));
  verticalLayout->addWidget(label);
}
