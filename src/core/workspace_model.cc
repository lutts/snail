// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/workspace_model.h"
#include "snail/i_work_model_factory.h"

// TODO(lutts): remove these header while remove the delete logic bellow
#include "snail/i_work_model.h"
#include "snail/i_work.h"

namespace snailcore {

void WorkSpaceModel::createWork(const utils::U8String& work_name) {
  auto work_model = work_model_factory_->createWorkModel(work_name);
  WorkModelAdded(work_model);
}

void WorkSpaceModel::removeWorkModel(IWorkModel* model) {
  (void)model;
}

}  // namespace snailcore
