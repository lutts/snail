// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_ATTRIBUTE_MODEL_H_
#define I_ATTRIBUTE_MODEL_H_

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IAttributeModel : public pfmvp::IPfModel {
 public:
  virtual ~IAttributeModel() = default;

  DEF_MODEL_ID(AttributeModel);
};

}  // namespace snailcore

#endif  // I_ATTRIBUTE_MODEL_H_
