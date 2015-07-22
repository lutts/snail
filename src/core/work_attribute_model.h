// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_WORK_ATTRIBUTE_MODEL_H_
#define SRC_CORE_WORK_ATTRIBUTE_MODEL_H_

#include <vector>

#include "utils/basic_utils.h"
#include "snail/i_work_attribute_model.h"
#include "utils/signal_slot_impl.h"

namespace utils {
class IRelayCommandFactory;
}  // namespace utils

namespace snailcore {

class IAttributeContainer;
class IAttributeModelFactory;

class WorkAttributeModel : public IWorkAttributeModel {
 public:
  WorkAttributeModel(IAttributeContainer* attr_container,
                     const IAttributeModelFactory& attr_model_factory,
                     const utils::IRelayCommandFactory& cmd_factory);
  virtual ~WorkAttributeModel() = default;

  bool isEditMode() const override;
  void switchToEditMode() override;
  void switchToDisplayMode() override;

  std::vector<std::shared_ptr<IAttributeModel>>
  getAttributeModels() const override;

  std::vector<utils::Command*> getEraseCommands() const override;
  std::vector<utils::Command*> getPopupEditorCommands() const override;
  std::vector<utils::Command*> getAddAttributeCommands() const override;

  Location getLocation(IPfModel* model) const override;
  Location getLocation(utils::Command* command) const override;

 private:
  SNAIL_DISABLE_COPY(WorkAttributeModel);

  IAttributeContainer* attr_container_;
  const IAttributeModelFactory& attr_model_factory_;
  const utils::IRelayCommandFactory& cmd_factory_;

 private:
  SNAIL_SIGSLOT_IMPL(AttributesChanged);
  SNAIL_SIGSLOT_IMPL(ShowPopupFor);
};

}  // namespace snailcore

#endif  // SRC_CORE_WORK_ATTRIBUTE_MODEL_H_
