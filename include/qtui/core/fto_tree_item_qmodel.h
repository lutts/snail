// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_TREE_ITEM_QMODEL_H_
#define INCLUDE_QTUI_I_TREE_ITEM_QMODEL_H_

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

template <typename RealItemType>
class TreeItemQModel {
 public:
  virtual ~TreeItemQModel() = default;

#if 0
  static_assert(std::is_base_of<ITreeItem, RealItemType>::value,
                "RealItemType must be derived from ITreeItem");
#endif

  virtual QAbstractItemModel* qmodel() const = 0;

  virtual void setTreeItemProvider(ITreeItemProvider* item_provider) = 0;
  virtual RealItemType* indexToItem(const QModelIndex& index) const = 0;
  virtual QModelIndex itemToIndex(ITreeItem* item) const = 0;
  virtual bool isAddMore(const QModelIndex& index) const = 0;
  virtual void itemAdded(ITreeItem* new_item, ITreeItem* parent_item) = 0;
};

#else  // DISABLE_TEST_CODE

template <typename RealItemType>
class TreeItemQModel { };

#endif  // DISABLE_TEST_CODE

}  // namespace fto

#endif  // INCLUDE_QTUI_I_TREE_ITEM_QMODEL_H_
