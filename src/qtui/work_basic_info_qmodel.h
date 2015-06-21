// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_WORK_BASIC_INFO_QMODEL_H_
#define SRC_QTUI_WORK_BASIC_INFO_QMODEL_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "qtui/i_work_basic_info_qmodel.h"

namespace snailcore {
class IWorkModel;
}  // namespace snailcore

class WorkBasicInfoQModel : public QAbstractItemModel
                          , public IWorkBasicInfoQModel {
 public:
  explicit WorkBasicInfoQModel(snailcore::IWorkModel* work)
      : work_(work) { }
  ~WorkBasicInfoQModel() = default;

  QVariant data(const QModelIndex &index, int role) const;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

 private:
  WorkBasicInfoQModel(const WorkBasicInfoQModel& other) = delete;
  WorkBasicInfoQModel& operator=(const WorkBasicInfoQModel& other) = delete;

  bool isNameIndex(const QModelIndex& index) const;

  snailcore::IWorkModel* work_;
};


#endif  // SRC_QTUI_WORK_BASIC_INFO_QMODEL_H_
