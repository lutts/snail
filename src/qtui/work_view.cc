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
  auto verticalLayout = new QVBoxLayout(this);

  QSint::ActionPanel* action_panel = new QSint::ActionPanel(this);
  QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(action_panel->sizePolicy().hasHeightForWidth());
  action_panel->setSizePolicy(sizePolicy);

  // work name & description
  QSint::ActionGroup *group1 = action_panel->createGroup(QStringLiteral("[Work Name Here]"));
  group1->setHasEditButton(true);

  group1->addWidget(new QPushButton(QStringLiteral("PlaceHolder"), this));
  group1->addWidget(new QPushButton(QStringLiteral("PlaceHolder"), this));
  group1->addWidget(new QPushButton(QStringLiteral("PlaceHolder"), this));
  group1->addWidget(new QPushButton(QStringLiteral("PlaceHolder"), this));
  group1->addWidget(new QPushButton(QStringLiteral("PlaceHolder"), this));

  connect(group1, &QSint::ActionGroup::editButtonClicked,
          [group1, this]() {
            qDebug() << "edit button clicked";
            if (!group1_editing) {
              group1->openHeaderTextEditor();
              group1->setEditButtonText(QStringLiteral("[Done]"));
              group1_editing = true;
            } else {
              group1->closeHeaderTextEditor();
              group1->setEditButtonText(QStringLiteral("[Edit]"));
              group1_editing = false;
            }
          });

  action_panel->addStretch();

  verticalLayout->addWidget(action_panel);
}
