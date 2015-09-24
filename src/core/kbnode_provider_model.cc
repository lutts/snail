// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_provider_model.h"
#include "snail/i_kbnode_provider.h"

namespace snailcore {

KbNodeProviderModel::KbNodeProviderModel(IKbNodeProvider* kbnode_provider)
    : kbnode_provider_(kbnode_provider) { }

KbNodeProviderModel::~KbNodeProviderModel() = default;

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

void KbNodeProviderModel::addKbNode() {
  if (!isNewKbNodeNameValid())
    return;

  auto new_kbnode = kbnode_provider_->addKbNode(new_name_, new_kbnode_parent_);
  if (new_kbnode)
    KbNodeAdded(new_kbnode, new_kbnode_parent_);
}

}  // namespace snailcore
