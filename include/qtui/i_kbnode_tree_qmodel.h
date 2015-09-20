// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_TREE_QMODEL_H_
#define I_KBNODE_TREE_QMODEL_H_

namespace snailcore {
class IKbNode;
class IKbNodeProvider;
}  // namespace snailcore

using snailcore::IKbNode;
using snailcore::IKbNodeProvider;

class QModelIndex;

class IKbNodeTreeQModel {
 public:
  virtual ~IKbNodeTreeQModel() = default;

  virtual void setKbNodeProvider(IKbNodeProvider* kbnode_provider) = 0;
  virtual IKbNode* kbNodeOfIndex(const QModelIndex& index) const = 0;
  virtual bool isAddKbNode(const QModelIndex& index) const = 0;
  virtual void beginResetQModel() = 0;
  virtual void endResetQModel() = 0;
  virtual void kbNodeAdded(IKbNode* new_kbnode) = 0;
};

#endif  // I_KBNODE_TREE_QMODEL_H_
