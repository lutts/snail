// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_WORKSPACE_MODEL_H_
#define I_WORKSPACE_MODEL_H_

#include <memory>

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IWorkModel;

class IWorkSpaceModel : public pfmvp::IPfModel {
 public:
  virtual ~IWorkSpaceModel() = default;

  DEF_MODEL_ID(WorkSpaceModel);

  SNAIL_SIGSLOT2(WorkModelAdded, void(std::shared_ptr<IWorkModel> work_model));
  SNAIL_SIGSLOT2(ActiveWorkModelChanged, void(IWorkModel* work_model));
  SNAIL_SIGSLOT2(WorkModelActivelyRemoved, void(IWorkModel* work_model));

  virtual void createWork(const utils::U8String& work_name) = 0;
  virtual void removeWorkModel(IWorkModel* model) = 0;
};

}  // namespace snailcore

#endif  // I_WORKSPACE_MODEL_H_
