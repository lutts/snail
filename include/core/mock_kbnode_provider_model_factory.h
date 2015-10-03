// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_PROVIDER_MODEL_FACTORY_H_
#define MOCK_KBNODE_PROVIDER_MODEL_FACTORY_H_

#include "core/i_kbnode_provider_model_factory.h"

namespace snailcore {
namespace tests {

class MockKbNodeProviderModelFactory : public IKbNodeProviderModelFactory {
 public:
  MOCK_CONST_METHOD1(
      createKbNodeProviderModel,
      std::shared_ptr<IKbNodeProviderModel>(IKbNodeProvider* provider));
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_PROVIDER_MODEL_FACTORY_H_
