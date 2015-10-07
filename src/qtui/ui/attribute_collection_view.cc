// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/ui/attribute_collection_view.h"

#include <QTableView>
#include <QHeaderView>
#include "qsint/include/QSint"
#include "src/qtui/core/attribute_collection_qmodel.h"
#include "src/qtui/core/attribute_delegate.h"

AttributeCollectionView::AttributeCollectionView()
    : action_group_(new QSint::ActionGroup(tr("Attributes")))
    , attr_table_view_(new QTableView()) {
  action_group_->setHasEditButton(true);

  attr_table_view_->horizontalHeader()->hide();
  attr_table_view_->verticalHeader()->hide();
  attr_table_view_->setShowGrid(false);
  action_group_->addWidget(attr_table_view_);

  QObject::connect(action_group_, &QSint::ActionGroup::editButtonClicked,
                   [this]() {
                     UserSwitchMode();
                   });
  QObject::connect(attr_table_view_, &QTableView::clicked,
                   [this](const QModelIndex& index) {
                     if (index.column() == 1) {
                       UserMayClickAddAttribute(index.row());
                     }
                   });
}

QWidget* AttributeCollectionView::getWidget() {
  return action_group_;
}

void AttributeCollectionView::setQModel(IAttributeCollectionQModel* qmodel) {
  auto model = dynamic_cast<QAbstractItemModel*>(qmodel);
  attr_table_view_->setModel(model);
}

void AttributeCollectionView::setAttributeDelegate(
    IAttributeDelegate* attr_delegate) {
  auto delegate = dynamic_cast<QAbstractItemDelegate*>(attr_delegate);
  attr_table_view_->setItemDelegateForColumn(1, delegate);
}

void AttributeCollectionView::openAttributeEditor(int row) {
  auto model = attr_table_view_->model();
  auto index = model->index(row, 1);
  attr_table_view_->openPersistentEditor(index);
}

void AttributeCollectionView::closeAttributeEditor(int row) {
  auto model = attr_table_view_->model();
  auto index = model->index(row, 1);
  attr_table_view_->closePersistentEditor(index);
}

void AttributeCollectionView::switchToDisplayMode() {
  action_group_->setEditButtonText(QStringLiteral("[Edit]"));
}

void AttributeCollectionView::switchToEditMode() {
  action_group_->setEditButtonText(QStringLiteral("[Done]"));
}

void AttributeCollectionView::setSwitchModelButtonEnabled(bool enabled) {
  (void)enabled;
  // TODO(lutts): impl this method
}
