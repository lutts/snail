// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef WORK_MODEL_H_
#define WORK_MODEL_H_

#include "snail/i_work_model.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class WorkModel : public IWorkModel {
 public:
  WorkModel(IWork* work) : work_(work) { }
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

#endif  // WORK_MODEL_H_
