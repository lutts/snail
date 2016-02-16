// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/factory/work_model_factory.h"
#include "src/core/work_model.h"

namespace snailcore {

std::shared_ptr<IWorkModel> WorkModelFactoryImpl::createWorkModel() {
  return std::make_shared<WorkModel>(attr_set_model_factory_);
}

}  // namespace snailcore
