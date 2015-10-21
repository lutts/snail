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
class KbNodeManager;
class ISimpleKbNodeAdderModelFactory;

class KbNodeAttributeModel : public IKbNodeAttributeModel {
 public:
  KbNodeAttributeModel(
      IKbNodeAttribute* kbnode_attr,
      KbNodeManager* kbnode_manager,
      ISimpleKbNodeAdderModelFactory* simple_kbnode_adder_model_factory);
  virtual ~KbNodeAttributeModel();

  bool isValid() const override;

  ITreeItemProvider* getKbNodeProvider() const override;

  std::shared_ptr<ISimpleKbNodeAdderModel>
  createSimpleKbNodeAdderModel() const override;

  utils::U8String getKbNodeName() const override;
  void setKbNode(IKbNode* kbnode) override;
  int setKbNodeByName(const utils::U8String& name) override;

 private:
  SNAIL_DISABLE_COPY(KbNodeAttributeModel);

  void validateComplete(bool result);

  IKbNodeAttribute* kbnode_attr_;
  KbNodeManager* kbnode_manager_;
  ISimpleKbNodeAdderModelFactory* simple_kbnode_adder_model_factory_;

  mutable std::shared_ptr<ITreeItemProvider> kbnode_provider_;

  bool last_validate_result_ { true };

 private:
  SNAIL_SIGSLOT_IMPL(ValidateComplete);
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_ATTRIBUTE_MODEL_H_
