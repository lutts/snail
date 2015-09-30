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
#include "src/core/kbnode_provider_model.h"

namespace snailcore {

KbNodeAttributeModel::KbNodeAttributeModel(IKbNodeAttribute* kbnode_attr,
                                           IKbNodeManager* kbnode_manager)
    : kbnode_attr_(kbnode_attr)
    , kbnode_manager_(kbnode_manager) { }

KbNodeAttributeModel::~KbNodeAttributeModel() { }

bool KbNodeAttributeModel::isValid() const {
  return last_validate_result_;
}

// NOTE: validate and attr empty is not the same thing
void KbNodeAttributeModel::validateComplete(bool result) {
  last_validate_result_ = result;
  ValidateComplete();
}

IKbNodeProvider* KbNodeAttributeModel::getKbNodeProvider() const {
  if (!kbnode_provider_) {
    auto attr_supplier = kbnode_attr_->kbnode_supplier();
    kbnode_provider_ = kbnode_manager_->createKbNodeProvider(
        attr_supplier->getRootKbNode());
  }

  return kbnode_provider_.get();
}

std::shared_ptr<IKbNodeProviderModel>
KbNodeAttributeModel::createKbNodeProviderModel() const {
  return std::make_shared<KbNodeProviderModel>(getKbNodeProvider());
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

    // if kbnode is nullptr, the validate result is true, but attr will be empty
    validateComplete(true);
  }
}

// NOTE: validate false should not update the underlying attribute
int KbNodeAttributeModel::setKbNodeByName(const utils::U8String& name) {
  getKbNodeProvider();

  if (name == kbnode_attr_->valueText()) {
    if (kbnode_provider_) {
      kbnode_provider_->setFilterPattern("");
    }

    validateComplete(true);
    return kSetKbNodeSuccess;
  } else if (kbnode_provider_) {
    auto kbnodes = kbnode_provider_->findKbNodeByName(name);
    if (kbnodes.empty()) {
      validateComplete(false);
      return kSetKbNodeNotFound;
    } else if (kbnodes.size() == 1) {
      auto kbnode = kbnodes[0];
      kbnode_attr_->setKbNode(kbnode);
      kbnode_provider_->setFilterPattern("");
      kbnode_provider_->incRef(kbnode);

      validateComplete(true);
      return kSetKbNodeSuccess;
    } else {  // multi match
      validateComplete(false);
      return kSetKbNodeMultpicMatched;
    }
  }

  return 0;
}

}  // namespace snailcore
