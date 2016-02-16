// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_FACTORY_WORK_FACTORY_H_
#define SRC_CORE_FACTORY_WORK_FACTORY_H_

#include "core/factory/i_work_factory.h"
#include "utils/basic_utils.h"

namespace snailcore {

FTO_BEGIN_NAMESPACE
class KbNodeManager;
FTO_END_NAMESPACE

class WorkFactory : public IWorkFactory {
 public:
  explicit WorkFactory(fto::KbNodeManager* kbnode_manager);
  virtual ~WorkFactory() = default;

  fto::Work* create(const utils::U8String& work_name) const override;

 private:
  SNAIL_DISABLE_COPY(WorkFactory);

  fto::KbNodeManager* kbnode_manager_;
};

}  // namespace snailcore

#endif  // SRC_CORE_FACTORY_WORK_FACTORY_H_
