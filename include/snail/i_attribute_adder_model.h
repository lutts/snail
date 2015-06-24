// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_ATTRIBUTE_ADDER_MODEL_H_
#define I_ATTRIBUTE_ADDER_MODEL_H_

#include <vector>
#include <memory>

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"
#include "utils/u8string.h"

namespace snailcore {

class IAttribute;
class IAttributeModel;

class IAttributeAdderModel : public pfmvp::IPfModel {
 public:
  virtual ~IAttributeAdderModel() = default;

  DEF_MODEL_ID(AttributeAdderModel);

  SNAIL_SIGSLOT2(ValidateComplete, void(bool result));
  SNAIL_SIGSLOT2(DiscardAttributeModel, void(IAttributeModel*));
  SNAIL_SIGSLOT2(CurrentAttributeModelChanged,
                 void(std::shared_ptr<IAttributeModel>));

  virtual utils::U8String getPrompt() const = 0;
  virtual std::vector<IAttribute*> getAllowedAttributeList() const = 0;
  virtual int getCurrentAttributeIndex() const = 0;
  virtual void setCurrentAttributeIndex(int index) = 0;
  virtual std::shared_ptr<IAttributeModel> getCurrentAttributeModel() = 0;

  virtual void doAddAttribute() = 0;
};

}  // namespace snailcore

#endif  // I_ATTRIBUTE_ADDER_MODEL_H_
