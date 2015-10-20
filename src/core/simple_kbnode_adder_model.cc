// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/simple_kbnode_adder_model.h"
#include "snail/i_tree_item_provider.h"
#include "core/i_kbnode_manager.h"

namespace snailcore {

SimpleKbNodeAdderModel::SimpleKbNodeAdderModel(ITreeItemProvider* kbnode_provider,
                                         IKbNodeManager* kbnode_manager)
    : kbnode_provider_(kbnode_provider)
    , kbnode_manager_(kbnode_manager) { }

SimpleKbNodeAdderModel::~SimpleKbNodeAdderModel() = default;

utils::U8String SimpleKbNodeAdderModel::name() const {
  return kbnode_provider_->name();
}

ITreeItemProvider* SimpleKbNodeAdderModel::getKbNodeProvider() const {
  return kbnode_provider_;
}

utils::U8String SimpleKbNodeAdderModel::getFilterPattern() const {
  return kbnode_provider_->getFilterPattern();
}

void SimpleKbNodeAdderModel::setFilterPattern(
    const utils::U8String& filter_pattern) {
  kbnode_provider_->setFilterPattern(filter_pattern);
}

IKbNode* SimpleKbNodeAdderModel::getNewKbNodeParent() const {
  return new_kbnode_parent_;
}

void SimpleKbNodeAdderModel::setNewKbNodeParent(IKbNode* kbnode) {
  new_kbnode_parent_ = kbnode;
}

void SimpleKbNodeAdderModel::setNewKbNodeName(const utils::U8String& name) {
  new_name_ = name;
}

bool SimpleKbNodeAdderModel::isNewKbNodeNameValid() const {
  return new_name_ != "";
}

void SimpleKbNodeAdderModel::setIsCategory(bool category) {
  is_category_ = category;
}

void SimpleKbNodeAdderModel::addKbNode() {
  if (!isNewKbNodeNameValid())
    return;

  IKbNode* actual_kbnode_parent = new_kbnode_parent_;
  if (!actual_kbnode_parent)
    actual_kbnode_parent = kbnode_provider_->getRootItem();

  auto new_kbnode = kbnode_manager_->addKbNode(new_name_,
                                               actual_kbnode_parent,
                                               is_category_);
  if (new_kbnode)
    KbNodeAdded(new_kbnode, new_kbnode_parent_);
}

}  // namespace snailcore
