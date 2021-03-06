// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/simple_kbnode_adder_model.h"
#include "snail/i_tree_item_provider.h"
#include "core/fto_kbnode_manager.h"
#include "core/i_kbnode.h"

namespace snailcore {

SimpleKbNodeAdderModel::SimpleKbNodeAdderModel(
    ITreeItemProvider* kbnode_provider, fto::KbNodeManager* kbnode_manager)
    : kbnode_provider_(kbnode_provider), kbnode_manager_(kbnode_manager) {}

SimpleKbNodeAdderModel::~SimpleKbNodeAdderModel() = default;

utils::U8String SimpleKbNodeAdderModel::name() const {
  return kbnode_provider_->name();
}

ITreeItemProvider* SimpleKbNodeAdderModel::getKbNodeProvider() const {
  return kbnode_provider_;
}

utils::U8String SimpleKbNodeAdderModel::getDefaultNewKbNodeName() const {
  auto default_name = kbnode_provider_->getFilterPattern();
  kbnode_provider_->setFilterPattern("");
  return default_name;
}

ITreeItem* SimpleKbNodeAdderModel::getNewKbNodeParent() const {
  return new_kbnode_parent_;
}

void SimpleKbNodeAdderModel::setNewKbNodeParent(ITreeItem* kbnode) {
  new_kbnode_parent_ = static_cast<IKbNode*>(kbnode);
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
  if (!isNewKbNodeNameValid()) return;

  IKbNode* actual_kbnode_parent = new_kbnode_parent_;
  if (!actual_kbnode_parent) {
    actual_kbnode_parent =
        static_cast<IKbNode*>(kbnode_provider_->getRootItem());
  }

  kbnode_manager_->addKbNode(new_name_, actual_kbnode_parent, is_category_);
}

}  // namespace snailcore
