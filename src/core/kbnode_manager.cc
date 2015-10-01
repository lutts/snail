// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_manager.h"

#include <vector>
#include "src/core/kbnode_provider.h"
#include "src/core/kbnode.h"

namespace snailcore {

KbNodeManager::KbNodeManager()
    : dummy_root_(new KbNode(0, "", false)) { }

KbNodeManager::~KbNodeManager() {
  // NOTE: we do not need to delete the nodes, because KbNodeManager will be a
  // singleton, and will not be destruct until the program is exiting
  // but to make valgrid happy, we delete the tree here
  erase(dummy_root_);
  delete dummy_root_;
}

void KbNodeManager::erase(IKbNode* parent) {
  for (auto & kbnode : kbnode_to_subnodes_[parent]) {
    erase(kbnode);
    delete kbnode;
  }
}

std::shared_ptr<IKbNodeProvider>
KbNodeManager::createKbNodeProvider(IKbNode* root_kbnode) {
  return std::make_shared<KbNodeProvider>(root_kbnode, this);
}

IKbNode* KbNodeManager::idToKbNode(KbNodeIdType kbnode_id) {
  IKbNode* kbnode = nullptr;
  if (id_to_kbnode_.find(kbnode_id) != id_to_kbnode_.end())
    kbnode = id_to_kbnode_[kbnode_id];

  return kbnode;
}

std::vector<IKbNode*>
KbNodeManager::findKbNode(const utils::U8String& pattern,
                          const IKbNode* parent_kbnode) {
  std::vector<IKbNode*> matched_kbnodes;

  if (!parent_kbnode)
    parent_kbnode = dummy_root_;

  auto iter = kbnode_to_subnodes_.find(parent_kbnode);
  if (iter != kbnode_to_subnodes_.end()) {
    auto & subnodes = kbnode_to_subnodes_[parent_kbnode];
    for (auto kbnode : subnodes) {
      if (!kbnode->isCategory() &&
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

std::vector<IKbNode*> KbNodeManager::childNodes(const IKbNode* parent_node) {
  if (!parent_node)
    parent_node = dummy_root_;

  return kbnode_to_subnodes_[parent_node];
}

KbNodeIdType KbNodeManager::nextId() {
  return ++id;
}

IKbNode* KbNodeManager::addKbNode(
    const utils::U8String& name, const IKbNode* parent, bool is_category) {
  if (!parent)
    parent = dummy_root_;

  auto kbnode = new KbNode(nextId(), name, is_category);
  id_to_kbnode_[kbnode->id()] = kbnode;
  kbnode_to_subnodes_[parent].push_back(kbnode);

  return kbnode;
}

void KbNodeManager::incRef(IKbNode* kbnode) {
  (void)kbnode;
  // TODO(lutts): currently not implemented
}


}  // namespace snailcore