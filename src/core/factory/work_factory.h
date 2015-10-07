// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_FACTORY_WORK_FACTORY_H_
#define SRC_CORE_FACTORY_WORK_FACTORY_H_

#include "core/i_work_factory.h"
#include "utils/basic_utils.h"

namespace snailcore {

class KbNodeManager;

class WorkFactory : public IWorkFactory {
 public:
  explicit WorkFactory(KbNodeManager* kbnode_manager);
  virtual ~WorkFactory() = default;

  IWork* createWork(const utils::U8String& work_name) override;

 private:
  SNAIL_DISABLE_COPY(WorkFactory);

  KbNodeManager* kbnode_manager_;
};


}  // namespace snailcore

#endif  // SRC_CORE_FACTORY_WORK_FACTORY_H_
