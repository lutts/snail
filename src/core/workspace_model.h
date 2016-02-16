// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_WORKSPACE_MODEL_H_
#define SRC_CORE_WORKSPACE_MODEL_H_

#include "snail/i_workspace_model.h"

namespace snailcore {

class IWorkModelFactory;
class IWorkFactory;

class WorkSpaceModelSignalHelper;

class WorkSpaceModel : public IWorkSpaceModel {
 public:
  explicit WorkSpaceModel(IWorkModelFactory* work_model_factory,
                          const IWorkFactory& work_factory);
  virtual ~WorkSpaceModel();

  void createWork(const utils::U8String& work_name) override;
  void removeWorkModel(IWorkModel* model) override;

 public:
  SNAIL_SIGSLOT_OVERRIDE(WorkModelAdded)
  SNAIL_SIGSLOT_OVERRIDE(ActiveWorkModelChanged)
  SNAIL_SIGSLOT_OVERRIDE(WorkModelActivelyRemoved)

 private:
  WorkSpaceModel(const WorkSpaceModel& other) = delete;
  WorkSpaceModel& operator=(const WorkSpaceModel& other) = delete;

  std::unique_ptr<WorkSpaceModelSignalHelper> signal_helper_;
  IWorkModelFactory* work_model_factory_;
  const IWorkFactory& work_factory_;
};

}  // namespace snailcore

#endif  // SRC_CORE_WORKSPACE_MODEL_H_
