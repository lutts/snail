// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef QMODELINDEX_GENERATOR_H_
#define QMODELINDEX_GENERATOR_H_

#include <cstdlib>

#include <QAbstractTableModel>

#include "utils/basic_utils.h"

class QModelIndexGenerator : public QAbstractTableModel {
 public:
  QModelIndexGenerator() { }
  virtual ~QModelIndexGenerator() { }

  using QAbstractTableModel::index;

  QVariant data(const QModelIndex &index, int role) const override {
    (void)index;
    (void)role;
    return QVariant();
  }

  int rowCount(const QModelIndex &parent = QModelIndex()) const override {
    (void)parent;
    return RAND_MAX;
  }
  int columnCount(const QModelIndex &parent = QModelIndex()) const override {
    (void)parent;
    return RAND_MAX;
  }

  QModelIndex index() const {
    return index(std::rand(), std::rand());
  }

 private:
  SNAIL_DISABLE_COPY(QModelIndexGenerator)
};


#endif  // QMODELINDEX_GENERATOR_H_
