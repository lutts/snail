// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_WORK_MODEL_FACTORY_H_
#define INCLUDE_CORE_I_WORK_MODEL_FACTORY_H_

#include <memory>

#include "include/config.h"

namespace snailcore {

FTO_BEGIN_NAMESPACE
class Work;
FTO_END_NAMESPACE

class IWorkModelFactory {
 public:
  virtual ~IWorkModelFactory() = default;

  virtual std::shared_ptr<IWorkModel> createWorkModel() = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_WORK_MODEL_FACTORY_H_
