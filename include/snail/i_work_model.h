// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_WORK_MODEL_H_
#define INCLUDE_SNAIL_I_WORK_MODEL_H_

#include "include/config.h"
#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"
#include "utils/u8string.h"

namespace snailcore {

FTO_BEGIN_NAMESPACE
class Work;
FTO_END_NAMESPACE
class IAttributeSetModel;

class IWorkModel : public pfmvp::IPfModel {
 public:
  virtual ~IWorkModel() = default;

  DEF_MODEL_ID(WorkModel);

  SNAIL_SIGSLOT_PURE_VIRTUAL(NameChanged,
                             void(const utils::U8String& new_name));

  virtual void set_work(fto::Work* work) = 0;

  virtual utils::U8String name() const = 0;
  virtual bool set_name(const utils::U8String& new_name) = 0;

  //
  virtual std::shared_ptr<IAttributeSetModel>
  createAttributeSetModel() = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_WORK_MODEL_H_
