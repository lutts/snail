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

#include "qtui/i_work_basic_info_qmodel.h"

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

  auto verticalSpacer = new QSpacerItem(20, 40,
                                        QSizePolicy::Minimum,
                                        QSizePolicy::Expanding);

  verticalLayout->addItem(verticalSpacer);
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
