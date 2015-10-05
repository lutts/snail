// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_WORK_MODEL_FACTORY_H_
#define SRC_CORE_WORK_MODEL_FACTORY_H_

#include <memory>

#include "core/i_work_model_factory.h"

#include "src/core/work.h"
#include "src/core/work_model.h"

namespace snailcore {

// TODO(lutts): move impl to .cc

class WorkModelFactory : public IWorkModelFactory {
 public:
  WorkModelFactory() = default;
  virtual ~WorkModelFactory() = default;

  std::shared_ptr<IWorkModel> createWorkModel(
      const utils::U8String& work_name) {
    auto work = new Work;
    work->set_name(work_name);  // TODO(lutts):
    auto model = std::make_shared<WorkModel>();
    model->set_work(work);
    return model;
  }

 private:
  WorkModelFactory(const WorkModelFactory& other) = delete;
  WorkModelFactory& operator=(const WorkModelFactory& other) = delete;
};


}  // namespace snailcore

#endif  // SRC_CORE_WORK_MODEL_FACTORY_H_
