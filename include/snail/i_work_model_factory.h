// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_WORK_MODEL_FACTORY_H_
#define I_WORK_MODEL_FACTORY_H_

#include <memory>

#include "utils/u8string.h"

namespace snailcore {

class IWorkModelFactory {
 public:
  virtual ~IWorkModelFactory() = default;

  virtual std::shared_ptr<IWorkModel> createWorkModel(
      const utils::U8String& work_name) = 0;
};

}  // namespace snailcore

#endif  // I_WORK_MODEL_FACTORY_H_
