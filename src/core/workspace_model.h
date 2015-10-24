// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_WORKSPACE_MODEL_H_
#define SRC_CORE_WORKSPACE_MODEL_H_

#include "snail/i_workspace_model.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class IWorkModelFactory;
class IWorkFactory;

class WorkSpaceModel : public IWorkSpaceModel {
 public:
  explicit WorkSpaceModel(
      IWorkModelFactory* work_model_factory,
      IWorkFactory* work_factory)
      : work_model_factory_(work_model_factory)
      , work_factory_(work_factory) { }
  virtual ~WorkSpaceModel() = default;

  void createWork(const utils::U8String& work_name) override;
  void removeWorkModel(IWorkModel* model);

 private:
  WorkSpaceModel(const WorkSpaceModel& other) = delete;
  WorkSpaceModel& operator=(const WorkSpaceModel& other) = delete;

  IWorkModelFactory* work_model_factory_;
  IWorkFactory* work_factory_;

 private:
  SNAIL_SIGSLOT_OVERRIDE(WorkModelAdded)
  SNAIL_SIGSLOT_OVERRIDE(ActiveWorkModelChanged)
  SNAIL_SIGSLOT_OVERRIDE(WorkModelActivelyRemoved)
};

}  // namespace snailcore

#endif  // SRC_CORE_WORKSPACE_MODEL_H_
