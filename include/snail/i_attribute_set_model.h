// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ATTRIBUTE_SET_MODEL_H_
#define INCLUDE_SNAIL_I_ATTRIBUTE_SET_MODEL_H_

#include <vector>

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IAttribute;
class IAttributeSupplier;
class IAttributeModel;

class IAttributeSetModel : public pfmvp::IPfModel {
 public:
  virtual ~IAttributeSetModel() = default;

  DEF_MODEL_ID(AttributeSetModel);

  SNAIL_SIGSLOT2(SwitchToEditMode, void());
  SNAIL_SIGSLOT2(SwitchToDisplayMode, void());
  SNAIL_SIGSLOT2(ValidateComplete, void(bool validate_result));

  virtual bool isEditMode() const = 0;
  virtual std::vector<IAttributeSupplier*> getAttributeSuppliers() const = 0;
  virtual void switchMode() = 0;
  virtual
  std::shared_ptr<IAttributeModel> createAttributeModel(IAttribute* attr) = 0;
  virtual void closeAttributeEditors(pfmvp::IPfTriadManager* triad_manager) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ATTRIBUTE_SET_MODEL_H_
