// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_TREE_ITEM_QMODEL_H_
#define INCLUDE_QTUI_MOCK_TREE_ITEM_QMODEL_H_

#include "qtui/i_tree_item_qmodel.h"

class MockTreeItemQModel : public ITreeItemQModel {
 public:
  MOCK_METHOD1(setTreeItemProvider, void(ITreeItemProvider* kbnode_provider));
  MOCK_CONST_METHOD1(indexToKbNode, IKbNode*(const QModelIndex& index));
  MOCK_CONST_METHOD1(kbNodeToIndex, QModelIndex(IKbNode* kbnode));
  MOCK_CONST_METHOD1(isAddMore, bool(const QModelIndex& index));
  MOCK_METHOD0(beginResetQModel, void());
  MOCK_METHOD0(endResetQModel, void());
  MOCK_METHOD2(kbNodeAdded, void(IKbNode* new_kbnode, IKbNode* parent_kbnode));
};

#endif  // INCLUDE_QTUI_MOCK_TREE_ITEM_QMODEL_H_
