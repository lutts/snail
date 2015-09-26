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

class KbNodeItem;

class KbNodeTreeQModelBase : public QAbstractItemModel
                       , public IKbNodeTreeQModel {
 public:
  KbNodeTreeQModelBase();
  virtual ~KbNodeTreeQModelBase();

  // IKbNodeTreeQModel
  void setKbNodeProvider(IKbNodeProvider* kbnode_provider) override;
  IKbNode* indexToKbNode(const QModelIndex& index) const override;
  QModelIndex kbNodeToIndex(IKbNode* kbnode) const override;
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

 protected:
  KbNodeItem* rootItem() const;
  QModelIndex itemToIndex(KbNodeItem* item) const;
  KbNodeItem* indexToItem(const QModelIndex& index) const;

  virtual void clear();
  virtual std::unique_ptr<KbNodeItem> createRootItem() const;

 private:
  SNAIL_DISABLE_COPY(KbNodeTreeQModelBase);

  mutable std::unique_ptr<KbNodeItem> root_item_;
};

class KbNodeTreeQModel : public KbNodeTreeQModelBase {
 public:
  KbNodeTreeQModel();
  ~KbNodeTreeQModel();

 private:
  bool isAddKbNode(const QModelIndex& index) const override;
  std::unique_ptr<KbNodeItem> createRootItem() const override;
};

class KbNodeTreeQModelWithProviderNode : public KbNodeTreeQModelBase {
 public:
  KbNodeTreeQModelWithProviderNode();
  ~KbNodeTreeQModelWithProviderNode();
  void setKbNodeProvider(IKbNodeProvider* kbnode_provider) override;

 private:
  void clear() override;
  QModelIndex kbNodeToIndex(IKbNode* kbnode) const override;

  KbNodeItem* provider_item_ { nullptr };
};


#endif  // SRC_QTUI_KBNODE_TREE_QMODEL_H_
