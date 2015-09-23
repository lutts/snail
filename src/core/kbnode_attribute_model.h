// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_ATTRIBUTE_MODEL_H_
#define SRC_CORE_KBNODE_ATTRIBUTE_MODEL_H_

#include "utils/basic_utils.h"
#include "snail/i_kbnode_attribute_model.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class IKbNodeAttribute;
class IKbNodeManager;

class KbNodeAttributeModel : public IKbNodeAttributeModel {
 public:
  KbNodeAttributeModel(IKbNodeAttribute* kbnode_attr,
                       IKbNodeManager* kbnode_manager);
  virtual ~KbNodeAttributeModel();

  IKbNodeProvider* getKbNodeProvider() const override;
  utils::U8String getKbNodeName() const override;
  void setKbNode(IKbNode* kbnode) override;
  int setKbNodeByName(const utils::U8String& name) override;

 private:
  SNAIL_DISABLE_COPY(KbNodeAttributeModel);

  IKbNodeAttribute* kbnode_attr_;
  IKbNodeManager* kbnode_manager_;
  mutable std::shared_ptr<IKbNodeProvider> kbnode_provider_;

 private:
  SNAIL_SIGSLOT_IMPL(ValidateComplete);
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_ATTRIBUTE_MODEL_H_
