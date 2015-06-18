// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef WORKSPACE_MODEL_H_
#define WORKSPACE_MODEL_H_

#include "snail/i_workspace_model.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class IWorkModelFactory;

class WorkSpaceModel : public IWorkSpaceModel {
 public:
  WorkSpaceModel(IWorkModelFactory* work_model_factory)
      : work_model_factory_(work_model_factory) { }
  virtual ~WorkSpaceModel() = default;

  void createWork(const utils::U8String& work_name) override;
  void removeWorkModel(IWorkModel* model);

 private:
  WorkSpaceModel(const WorkSpaceModel& other) = delete;
  WorkSpaceModel& operator=(const WorkSpaceModel& other) = delete;

  IWorkModelFactory* work_model_factory_;

 private:
  SNAIL_SIGSLOT_IMPL(WorkModelAdded)
  SNAIL_SIGSLOT_IMPL(ActiveWorkModelChanged)
  SNAIL_SIGSLOT_IMPL(WorkModelActivelyRemoved)
};

}  // namespace snailcore

#endif  // WORKSPACE_MODEL_H_
