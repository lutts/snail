// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_WORK_MODEL_H_
#define SRC_CORE_WORK_MODEL_H_

#include "snail/i_work_model.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class WorkModel : public IWorkModel {
 public:
  explicit WorkModel(IWork* work) : work_(work) { }
  virtual ~WorkModel() = default;

  utils::U8String name() const override;
  IWork* getWork() const override;

 private:
  WorkModel(const WorkModel& other) = delete;
  WorkModel& operator=(const WorkModel& other) = delete;

  IWork* work_;

 private:
  SNAIL_SIGSLOT_IMPL(BasicInfoChanged);
};

}  // namespace snailcore

#endif  // SRC_CORE_WORK_MODEL_H_
