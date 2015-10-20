// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_PROVIDER_FACTORY_H_
#define INCLUDE_CORE_MOCK_KBNODE_PROVIDER_FACTORY_H_

#include "core/i_kbnode_provider_factory.h"

namespace snailcore {
namespace tests {

class MockTreeItemProviderFactory : public ITreeItemProviderFactory {
 public:
  MOCK_CONST_METHOD2(
      createTreeItemProvider,
      std::shared_ptr<ITreeItemProvider>(IKbNode* root_kbnode,
                                       IKbNodeManager* kbnode_manager));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_KBNODE_PROVIDER_FACTORY_H_
