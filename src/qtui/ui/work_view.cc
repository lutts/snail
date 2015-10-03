// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/ui/work_view.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>

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

  name_desc_group_ = action_panel->createGroup(QStringLiteral("[Work Name Here]"));
  name_desc_group_->setHasEditButton(true);

  name_desc_group_->addWidget(new QLabel(QStringLiteral("PlaceHolder"), this));

  connect(name_desc_group_, SIGNAL(editButtonClicked()),
          this, SLOT(toggleNameDescEditMode()));

  parts_group_ = action_panel->createGroup(tr("Parts"));
  parts_group_->addWidget(new QLabel(QStringLiteral("PlaceHolder"), this));

  scores_group_ = action_panel->createGroup(tr("Scores"));
  scores_group_->addWidget(new QLabel(QStringLiteral("PlaceHolder"), this));
  scores_group_->setHasEditButton(true);

  recordings_group_ = action_panel->createGroup(tr("Recordings"));
  recordings_group_->addWidget(new QLabel(QStringLiteral("PlaceHolder"), this));
  recordings_group_->setHasEditButton(true);

  related_work_group_ = action_panel->createGroup(tr("Related Work(s)"));
  related_work_group_->addWidget(new QLabel(QStringLiteral("PlaceHolder"), this));

  action_panel->addStretch();

  verticalLayout->addWidget(action_panel);
}

void WorkView::toggleNameDescEditMode() {
  if (!name_desc_group_editing_) {
    name_desc_group_->openHeaderTextEditor();
    name_desc_group_->setEditButtonText(QStringLiteral("[Done]"));
    name_desc_group_editing_ = true;
  } else {
    name_desc_group_->closeHeaderTextEditor();
    name_desc_group_->setEditButtonText(QStringLiteral("[Edit]"));
    name_desc_group_editing_ = false;

    QString new_work_name = name_desc_group_->headerText();
    UserSetWorkName(new_work_name);
  }
}

void WorkView::setWorkName(const QString& work_name) {
  name_desc_group_->setHeaderText(work_name);
}
