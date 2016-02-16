// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_WORK_FACTORY_H_
#define INCLUDE_CORE_MOCK_WORK_FACTORY_H_

#include "core/factory/i_work_factory.h"

namespace snailcore {
namespace tests {

class MockWorkFactory : public IWorkFactory {
 public:
  MOCK_CONST_METHOD1(create, fto::Work*(const utils::U8String& work_name));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_WORK_FACTORY_H_
