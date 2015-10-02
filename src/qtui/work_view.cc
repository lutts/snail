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

#include "qtui/i_work_basic_info_qmodel.h"

#include "qsint/include/QSint"

WorkView::WorkView() {
  QVBoxLayout *verticalLayout = new QVBoxLayout(this);

  basicInfoView = new QTableView(this);
  basicInfoView->verticalHeader()->setVisible(false);
  basicInfoView->horizontalHeader()->setVisible(false);
  basicInfoView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  basicInfoView->setShowGrid(false);
  basicInfoView->setSelectionMode(QAbstractItemView::NoSelection);
  basicInfoView->setFrameShape(QFrame::NoFrame);
  basicInfoView->setFrameShadow(QFrame::Plain);
  basicInfoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  basicInfoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  verticalLayout->addWidget(basicInfoView);

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

#if 0
  auto verticalSpacer = new QSpacerItem(20, 40,
                                        QSizePolicy::Minimum,
                                        QSizePolicy::Expanding);

  verticalLayout->addItem(verticalSpacer);
#endif
}

/*
  void MainWindow::adjustTableSize()
  {
  ui->tableView->resizeColumnToContents(0);
  ui->tableView->resizeColumnToContents(1);
  ui->tableView->resizeColumnToContents(2);

  QRect rect = ui->tableView->geometry();
  rect.setWidth(2 + ui->tableView->verticalHeader()->width() +
  ui->tableView->columnWidth(0) + ui->tableView->columnWidth(1) + ui->tableView->columnWidth(2));
  ui->tableView->setGeometry(rect);
  }
*/

void WorkView::setBasicInfoQModel(IWorkBasicInfoQModel* model) {
  auto tablemodel = dynamic_cast<QAbstractItemModel*>(model);
  if (basicInfoView && tablemodel) {
    basicInfoView->setModel(tablemodel);

    basicInfoView->resizeRowsToContents();
    basicInfoView->resizeColumnsToContents();

    int maxh = 0;

    for (int i = 0; i < tablemodel->rowCount(); ++i)
      maxh += basicInfoView->rowHeight(i);

    basicInfoView->setMinimumHeight(maxh);
    basicInfoView->setMaximumHeight(maxh);
  }
}
