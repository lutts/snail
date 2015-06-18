// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/work_model.h"
#include "snail/i_work.h"

namespace snailcore {

utils::U8String WorkModel::name() const {
  return work_->name();
}

IWork* WorkModel::getWork() const {
  return work_;
}

}  // namespace snailcore
