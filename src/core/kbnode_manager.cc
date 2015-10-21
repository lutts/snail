// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_manager.h"

#include <vector>

#include "src/core/kbnode_item_provider.h"
#include "src/core/kbnode.h"

namespace snailcore {

class KbNodeManagerImpl {
 public:
  KbNodeManagerImpl()
      : dummy_root_(new KbNode(0, "", false)) { }
  ~KbNodeManagerImpl() {
    // NOTE: we do not need to delete the nodes, because KbNodeManager will be a
    // singleton, and will not be destruct until the program is exiting
    // but to make valgrid happy, we delete the tree here
    erase(dummy_root_);
    delete dummy_root_;
  }

  IKbNode* idToKbNode(KbNodeIdType kbnode_id);
  std::vector<IKbNode*> findKbNode(const utils::U8String& pattern,
                                   const IKbNode* parent_kbnode);
  std::vector<IKbNode*> childItems(const IKbNode* parent_node);
  IKbNode* addKbNode(const utils::U8String& name,
                     const IKbNode* parent,
                     bool is_category);
  void incRef(IKbNode* kbnode);

 private:
  KbNodeIdType nextId() {
    return ++id;
  }

  void erase(IKbNode* parent) {
    for (auto & kbnode : kbnode_to_subnodes_[parent]) {
      erase(kbnode);
      delete kbnode;
    }
  }

  std::unordered_map<KbNodeIdType, IKbNode*> id_to_kbnode_;
  std::unordered_map<const IKbNode*, std::vector<IKbNode*> > kbnode_to_subnodes_; // NOLINT

  KbNodeIdType id { 0 };

  IKbNode* dummy_root_ { nullptr };
};

IKbNode* KbNodeManagerImpl::idToKbNode(KbNodeIdType kbnode_id) {
  IKbNode* kbnode = nullptr;
  if (id_to_kbnode_.find(kbnode_id) != id_to_kbnode_.end())
    kbnode = id_to_kbnode_[kbnode_id];

  return kbnode;
}

std::vector<IKbNode*>
KbNodeManagerImpl::findKbNode(const utils::U8String& pattern,
                          const IKbNode* parent_kbnode) {
  std::vector<IKbNode*> matched_kbnodes;

  if (!parent_kbnode)
    parent_kbnode = dummy_root_;

  auto iter = kbnode_to_subnodes_.find(parent_kbnode);
  if (iter != kbnode_to_subnodes_.end()) {
    auto & subnodes = kbnode_to_subnodes_[parent_kbnode];
    for (auto kbnode : subnodes) {
      if (!kbnode->isGroupOnly() &&
          kbnode->name().find(pattern) != utils::U8String::npos) {
        matched_kbnodes.push_back(kbnode);
      }

      auto matched_subnodes = findKbNode(pattern, kbnode);
      matched_kbnodes.insert(matched_kbnodes.end(),
                             matched_subnodes.begin(),
                             matched_subnodes.end());
    }
  }

  return matched_kbnodes;
}

std::vector<IKbNode*> KbNodeManagerImpl::childItems(const IKbNode* parent_node) {
  if (!parent_node)
    parent_node = dummy_root_;

  return kbnode_to_subnodes_[parent_node];
}

IKbNode* KbNodeManagerImpl::addKbNode(
    const utils::U8String& name, const IKbNode* parent, bool is_category) {
  if (!parent)
    parent = dummy_root_;

  auto kbnode = new KbNode(nextId(), name, is_category);
  id_to_kbnode_[kbnode->id()] = kbnode;
  kbnode_to_subnodes_[parent].push_back(kbnode);

  return kbnode;
}

void KbNodeManagerImpl::incRef(IKbNode* kbnode) {
  (void)kbnode;
  // TODO(lutts): currently not implemented
}

////////////////////////////////////////////////////////////////

KbNodeManager::KbNodeManager()
    : impl_(utils::make_unique<KbNodeManagerImpl>()) { }

KbNodeManager::~KbNodeManager() = default;

std::shared_ptr<ITreeItemProvider>
KbNodeManager::createTreeItemProvider(IKbNode* root_kbnode) {
  return std::make_shared<KbNodeItemProvider>(root_kbnode, this);
}

IKbNode* KbNodeManager::idToKbNode(KbNodeIdType kbnode_id) {
  return impl_->idToKbNode(kbnode_id);
}

std::vector<IKbNode*>
KbNodeManager::findKbNode(const utils::U8String& pattern,
                          const IKbNode* parent_kbnode) {
  return impl_->findKbNode(pattern, parent_kbnode);
}

std::vector<IKbNode*> KbNodeManager::childItems(const IKbNode* parent_node) {
  return impl_->childItems(parent_node);
}

IKbNode* KbNodeManager::addKbNode(
    const utils::U8String& name, const IKbNode* parent, bool is_category) {
  return impl_->addKbNode(name, parent, is_category);
}

void KbNodeManager::incRef(IKbNode* kbnode) {
  impl_->incRef(kbnode);
}


}  // namespace snailcore
