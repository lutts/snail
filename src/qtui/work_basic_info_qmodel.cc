// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include "src/qtui/work_basic_info_qmodel.h"
#include "snail/i_work_model.h"

QVariant WorkBasicInfoQModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  int row = index.row();
  int col = index.column();

  if (!hasIndex(row, col))
    return QVariant();

  if (isNameIndex(index)) {
    switch (role) {
      case Qt::DisplayRole:
      case Qt::EditRole:
        return U8StringToQString(work_->name());
        // TODO(lutts): text alignment are UI specific, move to a proxy model
      case Qt::TextAlignmentRole:
        return static_cast<int>(Qt::AlignCenter | Qt::AlignVCenter);
      default:
        break;
    }
  }

  return QVariant();
}

bool WorkBasicInfoQModel::setData(const QModelIndex &index,
                                  const QVariant &value,
                                  int role) {
  if (index.isValid() && role == Qt::EditRole) {
    if (isNameIndex(index)) {
      utils::U8String newName = QStringToU8String(value.toString());
      if (work_->set_name(newName)) {
        emit(dataChanged(index, index));
        return true;
      }
    }
  }

  return false;
}

Qt::ItemFlags WorkBasicInfoQModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractItemModel::flags(index)
      | Qt::ItemNeverHasChildren
      | Qt::ItemIsEditable;
}

QModelIndex WorkBasicInfoQModel::index(int row, int column,
                                       const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  return createIndex(row, column);
}

QModelIndex WorkBasicInfoQModel::parent(const QModelIndex &) const {
  return QModelIndex();
}

inline bool WorkBasicInfoQModel::isNameIndex(const QModelIndex& index) const {
  return (index.row() == 0) && (index.column() == 0);
}

int WorkBasicInfoQModel::rowCount(const QModelIndex &) const {
  return 1;
}

int WorkBasicInfoQModel::columnCount(const QModelIndex &) const {
  return 1;
}
