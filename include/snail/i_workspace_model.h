// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_WORKSPACE_MODEL_H_
#define INCLUDE_SNAIL_I_WORKSPACE_MODEL_H_

#include <memory>

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IWorkModel;

class IWorkSpaceModel : public pfmvp::IPfModel {
 public:
  virtual ~IWorkSpaceModel() = default;

  DEF_MODEL_ID(WorkSpaceModel);

  SNAIL_SIGSLOT_PURE_VIRTUAL(WorkModelAdded,
                             void(std::shared_ptr<IWorkModel> work_model));
  SNAIL_SIGSLOT_PURE_VIRTUAL(ActiveWorkModelChanged,
                             void(IWorkModel* work_model));
  SNAIL_SIGSLOT_PURE_VIRTUAL(WorkModelActivelyRemoved,
                             void(IWorkModel* work_model));

  virtual void createWork(const utils::U8String& work_name) = 0;
  virtual void removeWorkModel(IWorkModel* model) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_WORKSPACE_MODEL_H_
