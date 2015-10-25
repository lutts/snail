// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CORE_TREE_ITEM_QMODEL_H_
#define SRC_QTUI_CORE_TREE_ITEM_QMODEL_H_

#include <memory>

#include "include/config.h"
#include "src/qtui/core/tree_item_qmodel_impl.h"
#include "utils/basic_utils.h"
#include "utils/i_trackable.h"
#include "snail/i_tree_item_provider.h"
#include FTO_HEADER(qtui/core, tree_item_qmodel)

class TreeItemQModel
    : public FTO_INTERFACE(TreeItemQModel)
    , public utils::ITrackable
    , public std::enable_shared_from_this<TreeItemQModel> {
 public:
  TreeItemQModel()
      : TreeItemQModel(utils::make_unique<TreeItemQModelImpl>()) { }

  explicit TreeItemQModel(std::unique_ptr<TreeItemQModelImpl>&& qmodel)
      : qmodel_(std::move(qmodel)) { }
  virtual ~TreeItemQModel() = default;

  QAbstractItemModel* qmodel() const {
    return qmodel_.get();
  }

  // TreeItemQModel
  void setTreeItemProvider(ITreeItemProvider* item_provider) {
    qmodel_->setTreeItemProvider(item_provider);

    item_provider->whenBeginFilter(
        [this]() {
          qmodel_->beginResetQModel();
        },
        this->shared_from_this());

    item_provider->whenFinishFilter(
        [this]() {
          qmodel_->endResetQModel();
        },
        this->shared_from_this());

    item_provider->whenItemAdded(
        [this](const ITreeItem* new_item, const ITreeItem* parent_item) {
          qmodel_->itemAdded(new_item, parent_item);
        },
        this->shared_from_this());
  }

  ITreeItem* indexToItem(const QModelIndex& index) const {
    return qmodel_->indexToItem(index);
  }

  QModelIndex itemToIndex(const ITreeItem* item) const {
    return qmodel_->itemToIndex(item);
  }

  bool isAddMore(const QModelIndex& index) const {
    return qmodel_->isAddMore(index);
  }

  void itemAdded(const ITreeItem* new_item, const ITreeItem* parent_item) {
    qmodel_->itemAdded(new_item, parent_item);
  }

 private:
  SNAIL_DISABLE_COPY(TreeItemQModel);

  std::unique_ptr<TreeItemQModelImpl> qmodel_;
};

class TreeItemQModelWithClearAndAddMoreRow
    : public TreeItemQModel {
 public:
  TreeItemQModelWithClearAndAddMoreRow()
      : TreeItemQModel(
            utils::make_unique<TreeItemQModelImplWithClearAndAddMoreRow>()) { }
  ~TreeItemQModelWithClearAndAddMoreRow() = default;
};

class TreeItemQModelWithProviderRoot : public TreeItemQModel {
 public:
  TreeItemQModelWithProviderRoot()
      : TreeItemQModel(
            utils::make_unique<TreeItemQModelImplWithProviderRoot>()) { }
  ~TreeItemQModelWithProviderRoot() = default;
};


#endif  // SRC_QTUI_CORE_TREE_ITEM_QMODEL_H_
