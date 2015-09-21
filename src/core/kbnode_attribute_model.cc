// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_attribute_model.h"
#include "core/i_kbnode_attribute.h"
#include "core/i_kbnode_manager.h"
#include "snail/i_kbnode_provider.h"

namespace snailcore {

KbNodeAttributeModel::KbNodeAttributeModel(IKbNodeAttribute* kbnode_attr,
                                           IKbNodeManager* kbnode_manager)
    : kbnode_attr_(kbnode_attr)
    , kbnode_manager_(kbnode_manager) { }

KbNodeAttributeModel::~KbNodeAttributeModel() { }

IKbNodeProvider* KbNodeAttributeModel::getKbNodeProvider() const {
  if (!kbnode_provider_) {
    auto attr_supplier = kbnode_attr_->kbnode_supplier();
    kbnode_provider_ = kbnode_manager_->createKbNodeProvider(
        attr_supplier->getRootKbNodeId());
  }

  return kbnode_provider_.get();
}

utils::U8String KbNodeAttributeModel::getKbNodeName() const {
  return kbnode_attr_->valueText();
}

void KbNodeAttributeModel::setKbNode(IKbNode* kbnode) {
  getKbNodeProvider();

  if (kbnode_provider_) {
    kbnode_attr_->setKbNode(kbnode);
    // quit filter mode
    kbnode_provider_->setFilterPattern("");
    kbnode_provider_->incRef(kbnode);

    ValidateComplete(true);
  }
}

int KbNodeAttributeModel::setKbNodeByName(const utils::U8String& name) {
  getKbNodeProvider();

  if (name == kbnode_attr_->valueText()) {
    if (kbnode_provider_) {
      kbnode_provider_->setFilterPattern("");
    }
    ValidateComplete(true);
    return kSetKbNodeSuccess;
  } else if (kbnode_provider_) {
    auto kbnodes = kbnode_provider_->findKbNodeByName(name);
    if (kbnodes.empty()) {
      ValidateComplete(false);
      return kSetKbNodeNotFound;
    } else if (kbnodes.size() == 1) {
      auto kbnode = kbnodes[0];
      kbnode_attr_->setKbNode(kbnode);
      kbnode_provider_->setFilterPattern("");
      kbnode_provider_->incRef(kbnode);
      ValidateComplete(true);
      return kSetKbNodeSuccess;
    } else {  // multi match
      ValidateComplete(false);
      return kSetKbNodeMultpicMatched;
    }
  }

  return 0;
}

}  // namespace snailcore
