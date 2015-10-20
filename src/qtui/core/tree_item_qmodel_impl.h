// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CORE_TREE_ITEM_QMODEL_IMPL_H_
#define SRC_QTUI_CORE_TREE_ITEM_QMODEL_IMPL_H_

#include <QAbstractItemModel>

#include "utils/basic_utils.h"

namespace snailcore {
class ITreeItem;
class ITreeItemProvider;
}  // namespace snailcore

using snailcore::ITreeItem;
using snailcore::ITreeItemProvider;

class QtTreeItem;

class TreeItemQModelImpl : public QAbstractItemModel {
 public:
  TreeItemQModelImpl();
  virtual ~TreeItemQModelImpl();

  virtual void setTreeItemProvider(ITreeItemProvider* item_provider);
  ITreeItem* indexToItem(const QModelIndex& index) const;
  virtual QModelIndex itemToIndex(ITreeItem* item) const;
  virtual bool isAddMore(const QModelIndex& index) const;
  void beginResetQModel();
  void endResetQModel();
  void itemAdded(ITreeItem* new_item, ITreeItem* parent_item);

  // QAbstractItemModel
  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;

 protected:
  QtTreeItem* rootItem() const;
  QModelIndex qtItemToIndex(QtTreeItem* item) const;
  QtTreeItem* indexToQtItem(const QModelIndex& index) const;

  virtual QVariant itemData(QtTreeItem* item, int role) const;

  virtual void clear();
  virtual std::unique_ptr<QtTreeItem> createRootItem() const;

 private:
  SNAIL_DISABLE_COPY(TreeItemQModelImpl);

  mutable std::unique_ptr<QtTreeItem> root_item_;
};

class TreeItemQModelImplWithClearAndAddMoreRow : public TreeItemQModelImpl {
 public:
  TreeItemQModelImplWithClearAndAddMoreRow();
  ~TreeItemQModelImplWithClearAndAddMoreRow();

  QVariant itemData(QtTreeItem* item, int role) const override;

 private:
  bool isAddMore(const QModelIndex& index) const override;
  std::unique_ptr<QtTreeItem> createRootItem() const override;
};

class TreeItemQModelImplWithProviderRoot : public TreeItemQModelImpl {
 public:
  TreeItemQModelImplWithProviderRoot();
  ~TreeItemQModelImplWithProviderRoot();
  void setTreeItemProvider(ITreeItemProvider* item_provider) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

 private:
  void clear() override;
  QModelIndex itemToIndex(ITreeItem* item) const override;

  QtTreeItem* provider_item_ { nullptr };
};


#endif  // SRC_QTUI_CORE_TREE_ITEM_QMODEL_IMPL_H_
