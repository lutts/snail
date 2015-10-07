// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_WORK_FACTORY_H_
#define INCLUDE_CORE_I_WORK_FACTORY_H_

#include "utils/u8string.h"

namespace snailcore {

class IWork;

class IWorkFactory {
 public:
  virtual ~IWorkFactory() = default;

  virtual IWork* createWork(const utils::U8String& work_name) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_WORK_FACTORY_H_
