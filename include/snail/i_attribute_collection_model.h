// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_WORK_ATTRIBUTE_MODEL_H_
#define INCLUDE_SNAIL_I_WORK_ATTRIBUTE_MODEL_H_

#include <vector>

#include "pfmvp/pf_model_with_triad_manager.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IAttributeModel;
class IAttributeDisplayBlockVisitor;
class UpdateAttrLabelData;

class IAttributeCollectionModel : public pfmvp::PfModelWithTriadManager {
 public:
  virtual ~IAttributeCollectionModel() = default;
  DEF_MODEL_ID(AttributeCollectionModel);

  virtual bool isEditMode() const = 0;
  virtual void switchToEditMode() = 0;
  virtual void switchToDisplayMode() = 0;

  SNAIL_SIGSLOT2(AttributesChanged, void());
  SNAIL_SIGSLOT2(AttrLabelChanged, void(UpdateAttrLabelData label_data));

  virtual void traverseAttributes(IAttributeDisplayBlockVisitor* visitor) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_WORK_ATTRIBUTE_MODEL_H_
