// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_WORK_MODEL_H_
#define INCLUDE_SNAIL_I_WORK_MODEL_H_

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"
#include "utils/u8string.h"

namespace snailcore {

class IWork;
class IAttributeAdderModel;

class IWorkModel : public pfmvp::IPfModel {
 public:
  virtual ~IWorkModel() = default;

  DEF_MODEL_ID(WorkModel);

  SNAIL_SIGSLOT2(NameChanged, void(const utils::U8String& new_name));

  virtual utils::U8String name() const = 0;
  virtual bool set_name(const utils::U8String& new_name) = 0;

  virtual std::shared_ptr<IAttributeAdderModel>
  createAttributeAdderModel() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_WORK_MODEL_H_
