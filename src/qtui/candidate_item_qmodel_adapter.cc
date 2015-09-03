// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/candidate_item_qmodel_adapter.h"
#include "snail/candidate_item.h"

using namespace snailcore;  // NOLINT

void CandidateItemQModelAdapter::setCandidates(
    const CandidateItem& root_item) {
  root_item_ = &root_item;
}

const CandidateItem*
CandidateItemQModelAdapter::itemOfIndex(const QModelIndex& index) const {
  if (!index.isValid())
    return root_item_;

  return static_cast<const CandidateItem*>(index.internalPointer());
}

QVariant CandidateItemQModelAdapter::data(
    const QModelIndex &index, int role) const {
  (void)role;
  if (!index.isValid())
    return QVariant();

  if (!root_item_)
    return QVariant();

  auto item = itemOfIndex(index);

  switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
      return U8StringToQString(item->text());
    case Qt::ToolTipRole:
      return U8StringToQString(item->description());
    default:
      return QVariant();
  }
}

Qt::ItemFlags CandidateItemQModelAdapter::flags(
    const QModelIndex &index) const {
  auto flags = QAbstractItemModel::flags(index);
  auto item = itemOfIndex(index);
  if (!item->isSelectable())
    flags &= ~Qt::ItemIsSelectable;

  return flags;
}

QModelIndex CandidateItemQModelAdapter::index(
    int row, int column, const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  auto item = itemOfIndex(parent);
  const void* iptr = static_cast<const void*>((*item)[row]);
  return createIndex(row, column, const_cast<void*>(iptr));
}

QModelIndex CandidateItemQModelAdapter::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return QModelIndex();

  auto item = itemOfIndex(index);

  if (item == root_item_) {
    return QModelIndex();
  }

  auto parent_item = item->parent();
  int row = -1;
  if (parent_item != root_item_) {
    row = parent_item->row();
  }

  const void* iptr = static_cast<const void*>(parent_item);
  return createIndex(row, 0, const_cast<void*>(iptr));
}

int CandidateItemQModelAdapter::rowCount(const QModelIndex &parent) const {
  auto item = itemOfIndex(parent);
  return item->num_children();
}

int CandidateItemQModelAdapter::columnCount(const QModelIndex &parent) const {
  (void)parent;
  return 1;
}
