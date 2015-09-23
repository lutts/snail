// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_KBNODE_TREE_QMODEL_H_
#define INCLUDE_QTUI_MOCK_KBNODE_TREE_QMODEL_H_

#include "qtui/i_kbnode_tree_qmodel.h"

class MockKbNodeTreeQModel : public IKbNodeTreeQModel {
 public:
  MOCK_METHOD1(setKbNodeProvider, void(IKbNodeProvider* kbnode_provider));
  MOCK_CONST_METHOD1(kbNodeOfIndex, IKbNode*(const QModelIndex& index));
  MOCK_CONST_METHOD1(isAddKbNode, bool(const QModelIndex& index));
  MOCK_METHOD0(beginResetQModel, void());
  MOCK_METHOD0(endResetQModel, void());
  MOCK_METHOD2(kbNodeAdded, void(IKbNode* new_kbnode, IKbNode* parent_kbnode));
};

#endif  // INCLUDE_QTUI_MOCK_KBNODE_TREE_QMODEL_H_