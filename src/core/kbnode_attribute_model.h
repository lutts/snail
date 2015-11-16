// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_ATTRIBUTE_MODEL_H_
#define SRC_CORE_KBNODE_ATTRIBUTE_MODEL_H_

#include <memory>

#include "include/config.h"
#include "utils/basic_utils.h"
#include "snail/i_kbnode_attribute_model.h"

namespace snailcore {

FTO_BEGIN_NAMESPACE
class KbNodeManager;
class KbNodeAttribute;
FTO_END_NAMESPACE

class ISimpleKbNodeAdderModelFactory;

class KbNodeAttributeModelSignalHelper;

class KbNodeAttributeModel : public IKbNodeAttributeModel {
 public:
  KbNodeAttributeModel(
      fto::KbNodeAttribute* kbnode_attr,
      fto::KbNodeManager* kbnode_manager,
      ISimpleKbNodeAdderModelFactory* simple_kbnode_adder_model_factory);
  virtual ~KbNodeAttributeModel();

  bool isValid() const override;

  ITreeItemProvider* getKbNodeProvider() const override;

  std::shared_ptr<ISimpleKbNodeAdderModel>
  createSimpleKbNodeAdderModel() const override;

  utils::U8String getKbNodeName() const override;
  void setKbNode(ITreeItem* item) override;
  int setKbNodeByName(const utils::U8String& name) override;

 public:
  SNAIL_SIGSLOT_OVERRIDE(ValidateComplete);

 private:
  SNAIL_DISABLE_COPY(KbNodeAttributeModel);
  void validateComplete(bool result);

  std::unique_ptr<KbNodeAttributeModelSignalHelper> signal_helper_;

  fto::KbNodeAttribute* kbnode_attr_;
  fto::KbNodeManager* kbnode_manager_;
  ISimpleKbNodeAdderModelFactory* simple_kbnode_adder_model_factory_;

  mutable std::shared_ptr<ITreeItemProvider> kbnode_provider_;

  bool last_validate_result_ { true };
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_ATTRIBUTE_MODEL_H_
