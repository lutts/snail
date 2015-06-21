// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_WORK_H_
#define SRC_CORE_WORK_H_

#include "snail/i_work.h"
#include "utils/u8string.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class Work : public IWork {
 public:
  Work() = default;
  virtual ~Work() = default;

  bool set_name(const utils::U8String& new_name) {
    if (this->name_ != new_name) {
      name_ = new_name;
      BasicInfoChanged();
      return true;
    }

    return false;
  }
  const utils::U8String& name() const {
    return name_;
  }

 private:
  Work(const Work& other) = delete;
  Work& operator=(const Work& other) = delete;

  utils::U8String name_;

 private:
  SNAIL_SIGSLOT_IMPL(BasicInfoChanged);
};


}  // namespace snailcore

#endif  // SRC_CORE_WORK_H_
