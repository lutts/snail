// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_MANAGER_H_
#define INCLUDE_CORE_I_KBNODE_MANAGER_H_

#include <memory>

#include "core/kb_global.h"

namespace snailcore {

class IKbNodeManager {
 public:
  virtual ~IKbNodeManager() = default;

  virtual std::shared_ptr<IKbNodeProvider>
  createKbNodeProvider(KbNodeIdType root_kbnode_id) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_KBNODE_MANAGER_H_