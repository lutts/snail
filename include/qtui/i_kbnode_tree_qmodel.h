// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_KBNODE_TREE_QMODEL_H_
#define INCLUDE_QTUI_I_KBNODE_TREE_QMODEL_H_

namespace snailcore {
class IKbNode;
class ITreeItemProvider;
}  // namespace snailcore

using snailcore::IKbNode;
using snailcore::ITreeItemProvider;

class QModelIndex;

class IKbNodeTreeQModel {
 public:
  virtual ~IKbNodeTreeQModel() = default;

  virtual void setTreeItemProvider(ITreeItemProvider* kbnode_provider) = 0;
  virtual IKbNode* indexToKbNode(const QModelIndex& index) const = 0;
  virtual QModelIndex kbNodeToIndex(IKbNode* kbnode) const = 0;
  virtual bool isAddKbNode(const QModelIndex& index) const = 0;
  virtual void beginResetQModel() = 0;
  virtual void endResetQModel() = 0;
  virtual void kbNodeAdded(IKbNode* new_kbnode, IKbNode* parent_kbnode) = 0;
};

#endif  // INCLUDE_QTUI_I_KBNODE_TREE_QMODEL_H_
