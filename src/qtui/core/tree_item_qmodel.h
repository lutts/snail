// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CORE_TREE_ITEM_QMODEL_H_
#define SRC_QTUI_CORE_TREE_ITEM_QMODEL_H_

#include <QAbstractItemModel>

#include "qtui/i_tree_item_qmodel.h"
#include "utils/basic_utils.h"

class KbNodeItem;

class TreeItemQModel : public QAbstractItemModel
                       , public ITreeItemQModel {
 public:
  TreeItemQModel();
  virtual ~TreeItemQModel();

  // ITreeItemQModel
  void setTreeItemProvider(ITreeItemProvider* kbnode_provider) override;
  IKbNode* indexToKbNode(const QModelIndex& index) const override;
  QModelIndex kbNodeToIndex(IKbNode* kbnode) const override;
  bool isAddMore(const QModelIndex& index) const override;
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

 protected:
  KbNodeItem* rootItem() const;
  QModelIndex itemToIndex(KbNodeItem* item) const;
  KbNodeItem* indexToItem(const QModelIndex& index) const;

  virtual QVariant itemData(KbNodeItem* item, int role) const;

  virtual void clear();
  virtual std::unique_ptr<KbNodeItem> createRootItem() const;

 private:
  SNAIL_DISABLE_COPY(TreeItemQModel);

  mutable std::unique_ptr<KbNodeItem> root_item_;
};

class TreeItemQModelWithClearAndAddMoreRow : public TreeItemQModel {
 public:
  TreeItemQModelWithClearAndAddMoreRow();
  ~TreeItemQModelWithClearAndAddMoreRow();

  QVariant itemData(KbNodeItem* item, int role) const override;

 private:
  bool isAddMore(const QModelIndex& index) const override;
  std::unique_ptr<KbNodeItem> createRootItem() const override;
};

class TreeItemQModelWithProviderNode : public TreeItemQModel {
 public:
  TreeItemQModelWithProviderNode();
  ~TreeItemQModelWithProviderNode();
  void setTreeItemProvider(ITreeItemProvider* kbnode_provider) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

 private:
  void clear() override;
  QModelIndex kbNodeToIndex(IKbNode* kbnode) const override;

  KbNodeItem* provider_item_ { nullptr };
};


#endif  // SRC_QTUI_CORE_TREE_ITEM_QMODEL_H_
