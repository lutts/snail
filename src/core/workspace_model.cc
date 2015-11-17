// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/workspace_model.h"

#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"
#include "core/i_work_model_factory.h"
#include "snail/i_work_model.h"
#include "core/i_work_factory.h"

namespace snailcore {

class WorkSpaceModelSignalHelper {
 public:
  SNAIL_SIGSLOT_PIMPL(WorkSpaceModel, WorkModelAdded);

 public:
  SNAIL_SIGSLOT_PIMPL(WorkSpaceModel, ActiveWorkModelChanged);

 public:
  SNAIL_SIGSLOT_PIMPL(WorkSpaceModel, WorkModelActivelyRemoved);
};

SNAIL_SIGSLOT_DELEGATE2(WorkSpaceModel, WorkModelAdded);
SNAIL_SIGSLOT_DELEGATE2(WorkSpaceModel, ActiveWorkModelChanged);
SNAIL_SIGSLOT_DELEGATE2(WorkSpaceModel, WorkModelActivelyRemoved);

WorkSpaceModel::WorkSpaceModel(IWorkModelFactory* work_model_factory,
                               IWorkFactory* work_factory)
    : signal_helper_(utils::make_unique<WorkSpaceModelSignalHelper>()),
      work_model_factory_(work_model_factory),
      work_factory_(work_factory) {}

WorkSpaceModel::~WorkSpaceModel() = default;

void WorkSpaceModel::createWork(const utils::U8String& work_name) {
  auto work = work_factory_->createWork(work_name);
  auto work_model = work_model_factory_->createWorkModel();
  work_model->set_work(work);
  signal_helper_->emitWorkModelAdded(work_model);
}

void WorkSpaceModel::removeWorkModel(IWorkModel* model) { (void)model; }

}  // namespace snailcore
