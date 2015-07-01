// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_WORK_MODEL_FACTORY_H_
#define INCLUDE_SNAIL_MOCK_WORK_MODEL_FACTORY_H_

#include "snail/i_work_model_factory.h"

namespace snailcore {
namespace tests {

class MockWorkModelFactory : public IWorkModelFactory {
 public:
  MOCK_METHOD1(createWorkModel,
               std::shared_ptr<IWorkModel>(const utils::U8String& work_name));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_WORK_MODEL_FACTORY_H_