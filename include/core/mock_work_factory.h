// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_WORK_FACTORY_H_
#define MOCK_WORK_FACTORY_H_

#include "core/i_work_factory.h"

namespace snailcore {
namespace tests {

class MockWorkFactory : public IWorkFactory {
 public:
  MOCK_METHOD1(createWork, IWork*(const utils::U8String& work_name));
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_WORK_FACTORY_H_
