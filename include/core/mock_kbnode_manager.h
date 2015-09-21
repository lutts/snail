// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_MANAGER_H_
#define MOCK_KBNODE_MANAGER_H_

#include "core/i_kbnode_manager.h"

namespace snailcore {
namespace tests {

class MockKbNodeManager : public IKbNodeManager {
 public:
  MOCK_METHOD1(createKbNodeProvider,
               std::shared_ptr<IKbNodeProvider>(KbNodeIdType root_kbnode_id));
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_MANAGER_H_
