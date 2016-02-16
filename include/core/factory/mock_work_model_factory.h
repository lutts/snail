// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_WORK_MODEL_FACTORY_H_
#define INCLUDE_CORE_MOCK_WORK_MODEL_FACTORY_H_

#include "core/factory/i_work_model_factory.h"

namespace snailcore {
namespace tests {

class MockWorkModelFactory : public IWorkModelFactory {
 public:
  MOCK_METHOD0(createWorkModel, std::shared_ptr<IWorkModel>());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_WORK_MODEL_FACTORY_H_
