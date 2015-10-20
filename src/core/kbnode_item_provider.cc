// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_item_provider.h"
#include "core/i_kbnode_manager.h"
#include "snail/i_kbnode.h"

namespace snailcore {

namespace {
class KbNodeIterator :
      public ITreeItemProvider::IChildItemIterator {
 public:
  KbNodeIterator() = default;
  explicit KbNodeIterator(const std::vector<IKbNode*>& kbnodes)
      : kbnodes_(kbnodes) { }
  virtual ~KbNodeIterator() = default;

  bool hasNext() const {
    return cur_idx_ < kbnodes_.size();
  }

  ITreeItem* next() {
    if (!hasNext())
      return nullptr;

    return kbnodes_[cur_idx_++];
  }

 private:
  std::vector<IKbNode*> kbnodes_;
  size_t cur_idx_ { 0 };
};

}  // namespace


KbNodeItemProvider::KbNodeItemProvider(IKbNode* root_kbnode,
                               IKbNodeManager* node_manager)
    : node_manager_(node_manager)
    , root_kbnode_(root_kbnode) { }

KbNodeItemProvider::~KbNodeItemProvider() = default;

utils::U8String KbNodeItemProvider::name() const {
  if (root_kbnode_)
    return root_kbnode_->name();
  else
    return "";
}

ITreeItem* KbNodeItemProvider::getRootItem() const {
  return root_kbnode_;
}

void KbNodeItemProvider::setFilterPattern(const utils::U8String& filter_pattern) {
  if (root_kbnode_ == nullptr)
    return;

  if (filter_pattern_ == filter_pattern)
    return;

  BeginFilter();
  if (filter_pattern != "")
    matched_kbnodes_ = node_manager_->findKbNode(filter_pattern, root_kbnode_);
  FinishFilter();

  filter_pattern_ = filter_pattern;
}

utils::U8String KbNodeItemProvider::getFilterPattern() const {
  return filter_pattern_;
}

bool KbNodeItemProvider::isFilterMode() const {
  return filter_pattern_ != "";
}

std::unique_ptr<ITreeItemProvider::IChildItemIterator>
KbNodeItemProvider::childItems(ITreeItem* parent_item) const {
  IKbNode* parent_node = static_cast<IKbNode*>(parent_item);
  if (isFilterMode()) {
    if (parent_node == nullptr)
      return utils::make_unique<KbNodeIterator>(matched_kbnodes_);
    else
      return utils::make_unique<KbNodeIterator>();
  } else {
    if (parent_node == nullptr)
      parent_node = root_kbnode_;
    return utils::make_unique<KbNodeIterator>(
        node_manager_->childItems(parent_node));
  }
}

}  // namespace snailcore
