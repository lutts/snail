// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_manager.h"

#include <vector>

#include "utils/signal_slot_impl.h"
#include "src/core/kbnode_item_provider.h"
#include "src/core/kbnode.h"

namespace snailcore {

class KbNodeManagerSignalHelper {
 public:
  SNAIL_SIGSLOT_IMPL(KbNodeAdded, void(const IKbNode* new_kbnode,
                                       const IKbNode* parent_kbnode));

 public:
  void emitKbNodeAdded(const IKbNode* new_kbnode,
                       const IKbNode* parent_kbnode) {
    KbNodeAdded(new_kbnode, parent_kbnode);
  }
};

KbNodeManager::KbNodeManager()
    : signal_helper_(utils::make_unique<KbNodeManagerSignalHelper>()),
      dummy_root_(new KbNode(0, "", false)) {}

KbNodeManager::~KbNodeManager() {
  // NOTE: we do not need to delete the nodes, because KbNodeManager will be a
  // singleton, and will not be destruct until the program is exiting
  // but to make valgrid happy, we delete the tree here
  erase(dummy_root_);
  delete dummy_root_;
}

void KbNodeManager::erase(IKbNode* parent) {
  for (auto& kbnode : kbnode_to_subnodes_[parent]) {
    erase(kbnode);
    delete kbnode;
  }
}

std::shared_ptr<ITreeItemProvider> KbNodeManager::createTreeItemProvider(
    IKbNode* root_kbnode) {
  auto item_provider =
      utils::make_trackable<KbNodeItemProvider>(root_kbnode, this);
  auto raw_item_provider = item_provider.get();
  signal_helper_->whenKbNodeAdded(
      [raw_item_provider](const IKbNode* new_kbnode,
                          const IKbNode* parent_kbnode) {
        raw_item_provider->itemAdded(new_kbnode, parent_kbnode);
      },
      item_provider);

  return item_provider;
}

IKbNode* KbNodeManager::idToKbNode(KbNodeIdType kbnode_id) {
  IKbNode* kbnode = nullptr;
  if (id_to_kbnode_.find(kbnode_id) != id_to_kbnode_.end())
    kbnode = id_to_kbnode_[kbnode_id];

  return kbnode;
}

std::vector<IKbNode*> KbNodeManager::findKbNode(const utils::U8String& pattern,
                                                const IKbNode* parent_kbnode) {
  std::vector<IKbNode*> matched_kbnodes;

  if (!parent_kbnode) parent_kbnode = dummy_root_;

  auto iter = kbnode_to_subnodes_.find(parent_kbnode);
  if (iter != kbnode_to_subnodes_.end()) {
    auto& subnodes = kbnode_to_subnodes_[parent_kbnode];
    for (auto kbnode : subnodes) {
      if (!kbnode->isGroupOnly() &&
          kbnode->name().find(pattern) != utils::U8String::npos) {
        matched_kbnodes.push_back(kbnode);
      }

      auto matched_subnodes = findKbNode(pattern, kbnode);
      matched_kbnodes.insert(matched_kbnodes.end(), matched_subnodes.begin(),
                             matched_subnodes.end());
    }
  }

  return matched_kbnodes;
}

std::vector<IKbNode*> KbNodeManager::childItems(const IKbNode* parent_node) {
  if (!parent_node) parent_node = dummy_root_;

  return kbnode_to_subnodes_[parent_node];
}

KbNodeIdType KbNodeManager::nextId() { return ++id; }

IKbNode* KbNodeManager::addKbNode(const utils::U8String& name,
                                  const IKbNode* parent, bool is_category) {
  if (!parent) parent = dummy_root_;

  auto kbnode = new KbNode(nextId(), name, is_category);
  if (kbnode) {
    id_to_kbnode_[kbnode->id()] = kbnode;
    kbnode_to_subnodes_[parent].push_back(kbnode);
    signal_helper_->emitKbNodeAdded(kbnode, parent);
  }

  return kbnode;
}

}  // namespace snailcore
