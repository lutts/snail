// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_CORE_FTO_TREE_ITEM_QMODEL_H_
#define INCLUDE_QTUI_CORE_FTO_TREE_ITEM_QMODEL_H_

namespace snailcore {
class ITreeItem;
class ITreeItemProvider;
}  // namespace snailcore

using snailcore::ITreeItem;
using snailcore::ITreeItemProvider;

class QModelIndex;
class QAbstractItemModel;

namespace fto {

#ifndef DISABLE_TEST_CODE

class TreeItemQModel {
 public:
  virtual ~TreeItemQModel() = default;

  virtual QAbstractItemModel* qmodel() const = 0;

  virtual void setTreeItemProvider(ITreeItemProvider* item_provider) = 0;
  virtual ITreeItem* indexToItem(const QModelIndex& index) const = 0;
  virtual QModelIndex itemToIndex(const ITreeItem* item) const = 0;
  virtual bool isAddMore(const QModelIndex& index) const = 0;
  virtual void itemAdded(const ITreeItem* new_item,
                         const ITreeItem* parent_item) = 0;
};

#else  // DISABLE_TEST_CODE

class TreeItemQModel {};

#endif  // DISABLE_TEST_CODE

}  // namespace fto

#endif  // INCLUDE_QTUI_CORE_FTO_TREE_ITEM_QMODEL_H_
