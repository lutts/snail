// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CORE_TREE_ITEM_QMODEL_H_
#define SRC_QTUI_CORE_TREE_ITEM_QMODEL_H_

#include "src/qtui/core/tree_item_qmodel_impl.h"
#include "utils/basic_utils.h"
#include "qtui/i_tree_item_qmodel.h"

template <typename RealItemType>
class TreeItemQModel : public ITreeItemQModel<RealItemType> {
 public:
  TreeItemQModel()
      : TreeItemQModel(utils::make_unique<TreeItemQModelImpl>()) { }

  explicit TreeItemQModel(std::unique_ptr<TreeItemQModelImpl>&& qmodel)
      : qmodel_(std::move(qmodel)) { }
  virtual ~TreeItemQModel() = default;

  QAbstractItemModel* qmodel() const override {
    return qmodel_.get();
  }

  // ITreeItemQModel
  void setTreeItemProvider(ITreeItemProvider* item_provider) override {
    qmodel_->setTreeItemProvider(item_provider);
  }

  RealItemType* indexToItem(const QModelIndex& index) const override {
    return static_cast<RealItemType*>(qmodel_->indexToItem(index));
  }

  QModelIndex itemToIndex(ITreeItem* item) const override {
    return qmodel_->itemToIndex(item);
  }

  bool isAddMore(const QModelIndex& index) const {
    return qmodel_->isAddMore(index);
  }

  void beginResetQModel() override {
    return qmodel_->beginResetQModel();
  }

  void endResetQModel() override {
    return qmodel_->endResetQModel();
  }

  void itemAdded(ITreeItem* new_item, ITreeItem* parent_item) override {
    qmodel_->itemAdded(new_item, parent_item);
  }

 private:
  SNAIL_DISABLE_COPY(TreeItemQModel);

  std::unique_ptr<TreeItemQModelImpl> qmodel_;
};

template <typename RealItemType>
class TreeItemQModelWithClearAndAddMoreRow
    : public TreeItemQModel<RealItemType> {
 public:
  TreeItemQModelWithClearAndAddMoreRow()
      : TreeItemQModel<RealItemType>(
            utils::make_unique<TreeItemQModelImplWithClearAndAddMoreRow>()) { }
  ~TreeItemQModelWithClearAndAddMoreRow() = default;
};

template <typename RealItemType>
class TreeItemQModelWithProviderRoot : public TreeItemQModel<RealItemType> {
 public:
  TreeItemQModelWithProviderRoot()
      : TreeItemQModel<RealItemType>(
            utils::make_unique<TreeItemQModelImplWithProviderRoot>()) { }
  ~TreeItemQModelWithProviderRoot() = default;
};


#endif  // SRC_QTUI_CORE_TREE_ITEM_QMODEL_H_
