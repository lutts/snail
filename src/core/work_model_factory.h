// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef WORK_MODEL_FACTORY_H_
#define WORK_MODEL_FACTORY_H_

#include <memory>

#include "snail/i_work_model_factory.h"

#include "src/core/work.h"
#include "src/core/work_model.h"

namespace snailcore {

class WorkModelFactory : public IWorkModelFactory {
 public:
  WorkModelFactory() = default;
  virtual ~WorkModelFactory() = default;

  std::shared_ptr<IWorkModel> createWorkModel(
      const utils::U8String& work_name) {
    auto work = new Work;
    work->set_name(work_name);
    return std::make_shared<WorkModel>(work);
  }

 private:
  WorkModelFactory(const WorkModelFactory& other) = delete;
  WorkModelFactory& operator=(const WorkModelFactory& other) = delete;
};


}  // namespace snailcore

#endif  // WORK_MODEL_FACTORY_H_
