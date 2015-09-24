// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_KBNODE_TREE_QMODEL_H_
#define SRC_QTUI_KBNODE_TREE_QMODEL_H_

#include <QAbstractItemModel>

#include "qtui/i_kbnode_tree_qmodel.h"
#include "utils/basic_utils.h"

class KbNodeTreeQModelImpl;
class KbNodeItem;

class KbNodeTreeQModel : public QAbstractItemModel
                       , public IKbNodeTreeQModel {
 public:
  KbNodeTreeQModel();
  virtual ~KbNodeTreeQModel();

  // IKbNodeTreeQModel
  void enterAddNewNodeMode() override;
  void setKbNodeProvider(IKbNodeProvider* kbnode_provider) override;
  IKbNode* kbNodeOfIndex(const QModelIndex& index) const override;
  QModelIndex indexOfKbNode(IKbNode* kbnode) const override;
  bool isAddKbNode(const QModelIndex& index) const override;
  void beginResetQModel() override;
  void endResetQModel() override;
  void kbNodeAdded(IKbNode* new_kbnode, IKbNode* parent_kbnode) override;

  // QAbstractItemModel
  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;

 private:
  SNAIL_DISABLE_COPY(KbNodeTreeQModel);

  QModelIndex itemToIndex(KbNodeItem* item) const;
  KbNodeItem* indexToItem(const QModelIndex& index) const;

  std::unique_ptr<KbNodeTreeQModelImpl> pimpl;
  friend class KbNodeTreeQModelImpl;
};


#endif  // SRC_QTUI_KBNODE_TREE_QMODEL_H_
