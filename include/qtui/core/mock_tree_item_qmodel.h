// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_TREE_ITEM_QMODEL_H_
#define INCLUDE_QTUI_MOCK_TREE_ITEM_QMODEL_H_

#include "qtui/core/fto_tree_item_qmodel.h"

template <typename RealItemType>
class MockTreeItemQModel : public fto::TreeItemQModel<RealItemType> {
 public:
  MOCK_CONST_METHOD0(qmodel, QAbstractItemModel*());

  MOCK_METHOD1(setTreeItemProvider, void(ITreeItemProvider* item_provider));
  MOCK_CONST_METHOD1_T(indexToItem, RealItemType*(const QModelIndex& index));
  MOCK_CONST_METHOD1(itemToIndex, QModelIndex(const ITreeItem* item));
  MOCK_CONST_METHOD1(isAddMore, bool(const QModelIndex& index));
  MOCK_METHOD2(itemAdded, void(const ITreeItem* new_item,
                               const ITreeItem* parent_item));
};

#endif  // INCLUDE_QTUI_MOCK_TREE_ITEM_QMODEL_H_