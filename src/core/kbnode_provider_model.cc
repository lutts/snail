// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_provider_model.h"
#include "snail/i_kbnode_provider.h"
#include "core/i_kbnode_manager.h"

namespace snailcore {

KbNodeProviderModel::KbNodeProviderModel(IKbNodeProvider* kbnode_provider,
                                         IKbNodeManager* kbnode_manager)
    : kbnode_provider_(kbnode_provider)
    , kbnode_manager_(kbnode_manager) { }

KbNodeProviderModel::~KbNodeProviderModel() = default;

utils::U8String KbNodeProviderModel::name() const {
  return kbnode_provider_->name();
}

IKbNodeProvider* KbNodeProviderModel::getKbNodeProvider() const {
  return kbnode_provider_;
}

utils::U8String KbNodeProviderModel::getFilterPattern() const {
  return kbnode_provider_->getFilterPattern();
}

void KbNodeProviderModel::setFilterPattern(
    const utils::U8String& filter_pattern) {
  kbnode_provider_->setFilterPattern(filter_pattern);
}

IKbNode* KbNodeProviderModel::getNewKbNodeParent() const {
  return new_kbnode_parent_;
}

void KbNodeProviderModel::setNewKbNodeParent(IKbNode* kbnode) {
  new_kbnode_parent_ = kbnode;
}

void KbNodeProviderModel::setNewKbNodeName(const utils::U8String& name) {
  new_name_ = name;
}

bool KbNodeProviderModel::isNewKbNodeNameValid() const {
  return new_name_ != "";
}

void KbNodeProviderModel::setIsCategory(bool category) {
  is_category_ = category;
}

void KbNodeProviderModel::addKbNode() {
  if (!isNewKbNodeNameValid())
    return;

#if 0
  IKbNode* kbnode_parent = new_kbnode_parent_;
  if (!kbnode_parent)
    kbnode_parent = kbnode_provider_->getRootKbNode();
#endif

  auto new_kbnode = kbnode_manager_->addKbNode(new_name_,
                                                new_kbnode_parent_,
                                                is_category_);
  if (new_kbnode)
    KbNodeAdded(new_kbnode, new_kbnode_parent_);
}

}  // namespace snailcore
