// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
#define SRC_QTUI_CANDIDATE_ITEM_QMODEL_ADAPTER_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QDebug>

#include "qtui/i_candidate_item_qmodel_adapter.h"
#include "utils/basic_utils.h"

class CandidateItemQModelAdapter : public ICandidateItemQModelAdapter
                                , public QAbstractItemModel {
 public:
  CandidateItemQModelAdapter() = default;
  virtual ~CandidateItemQModelAdapter() = default;

  void setCandidates(const snailcore::CandidateItem& root_item) override;

  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

 private:
  SNAIL_DISABLE_COPY(CandidateItemQModelAdapter)

  const snailcore::CandidateItem* itemOfIndex(const QModelIndex& index) const;

  const snailcore::CandidateItem* root_item_ { nullptr };
};


#endif  // SRC_QTUI_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
