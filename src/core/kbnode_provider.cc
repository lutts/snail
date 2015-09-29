// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_provider.h"
#include "core/i_kbnode_manager.h"
#include "snail/i_kbnode.h"

namespace snailcore {

namespace {
class KbNodeIterator :
      public IKbNodeProvider::IChildNodeIterator {
 public:
  KbNodeIterator() = default;
  explicit KbNodeIterator(const std::vector<IKbNode*>& kbnodes)
      : kbnodes_(kbnodes) { }
  virtual ~KbNodeIterator() = default;

  bool hasNext() const {
    return cur_idx_ < kbnodes_.size();
  }

  IKbNode* next() {
    if (!hasNext())
      return nullptr;

    return kbnodes_[cur_idx_++];
  }

 private:
  std::vector<IKbNode*> kbnodes_;
  size_t cur_idx_ { 0 };
};

}  // namespace


KbNodeProvider::KbNodeProvider(IKbNode* root_kbnode,
                               IKbNodeManager* node_manager)
    : node_manager_(node_manager)
    , root_kbnode_(root_kbnode) { }

KbNodeProvider::~KbNodeProvider() = default;

utils::U8String KbNodeProvider::name() const {
  if (root_kbnode_)
    return root_kbnode_->name();
  else
    return "";
}

void KbNodeProvider::setFilterPattern(const utils::U8String& filter_pattern) {
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

utils::U8String KbNodeProvider::getFilterPattern() const {
  return filter_pattern_;
}

bool KbNodeProvider::isFilterMode() const {
  return filter_pattern_ != "";
}

IKbNode* KbNodeProvider::addKbNode(
    const utils::U8String& name, IKbNode* parent, bool is_category) {
  if (parent == nullptr)
    parent = root_kbnode_;
  return node_manager_->addKbNode(name, parent, is_category);
}

std::unique_ptr<IKbNodeProvider::IChildNodeIterator>
KbNodeProvider::childNodes(IKbNode* parent_node) const {
  if (isFilterMode()) {
    if (parent_node == nullptr)
      return utils::make_unique<KbNodeIterator>(matched_kbnodes_);
    else
      return utils::make_unique<KbNodeIterator>();
  } else {
    if (parent_node == nullptr)
      parent_node = root_kbnode_;
    return utils::make_unique<KbNodeIterator>(
        node_manager_->childNodes(parent_node));
  }
}

void KbNodeProvider::incRef(IKbNode* kbnode) {
  node_manager_->incRef(kbnode);
}

std::vector<IKbNode*>
KbNodeProvider::findKbNodeByName(const utils::U8String& name) {
  if (root_kbnode_ == nullptr)
    return std::vector<IKbNode*>();

  if (name.empty())
    return std::vector<IKbNode*>();

  if (name == filter_pattern_)
    return matched_kbnodes_;

  return node_manager_->findKbNode(name, root_kbnode_);
}

}  // namespace snailcore
