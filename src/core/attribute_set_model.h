// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_ATTRIBUTE_SET_MODEL_H_
#define SRC_CORE_ATTRIBUTE_SET_MODEL_H_

#include <vector>
#include <memory>

#include "snail/i_attribute_set_model.h"
#include "utils/basic_utils.h"

namespace snailcore {

class IAttributeModelFactory;

class AttributeSetModelSignalHelper;

class AttributeSetModel : public IAttributeSetModel {
 public:
  AttributeSetModel(const std::vector<IAttributeSupplier*>& attr_suppliers,
                    IAttributeModelFactory* attr_model_factory);
  virtual ~AttributeSetModel();

  bool isEditMode() const override { return edit_mode_; }
  std::vector<IAttributeSupplier*> getAttributeSuppliers() const override;
  void switchMode() override;

  std::shared_ptr<IAttributeModel>
  createAttributeModel(IAttribute* attr) override;

  void closeAttributeEditors(pfmvp::IPfTriadManager* triad_manager) override;

 public:
  SNAIL_SIGSLOT_OVERRIDE(SwitchToEditMode);
  SNAIL_SIGSLOT_OVERRIDE(SwitchToDisplayMode)
  SNAIL_SIGSLOT_OVERRIDE(ValidateComplete);

 private:
  SNAIL_DISABLE_COPY(AttributeSetModel);

  void switchToEditMode();
  void switchToDisplayMode();
  void validateComplete();

  std::unique_ptr<AttributeSetModelSignalHelper> signal_helper_;

  std::vector<IAttributeSupplier*> attr_suppliers_;
  IAttributeModelFactory* attr_model_factory_;

  bool edit_mode_ { false };
  std::vector<std::weak_ptr<IAttributeModel> > attr_models_;
};


}  // namespace snailcore

#endif  // SRC_CORE_ATTRIBUTE_SET_MODEL_H_
