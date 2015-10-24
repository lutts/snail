// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_WORK_FACTORY_H_
#define INCLUDE_CORE_I_WORK_FACTORY_H_

#include "include/config.h"
#include "utils/u8string.h"

namespace snailcore {

FTO_BEGIN_NAMESPACE
class Work;
FTO_END_NAMESPACE

class IWorkFactory {
 public:
  virtual ~IWorkFactory() = default;

  virtual fto::Work* createWork(const utils::U8String& work_name) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_WORK_FACTORY_H_
