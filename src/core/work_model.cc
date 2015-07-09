// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/work_model.h"
#include "snail/i_work.h"

namespace snailcore {

WorkModel::~WorkModel() {
  // TODO(lutts): delete work is not our responsibility,
  // remove these codes some day
  // delete work_;
}

void WorkModel::set_work(IWork* work) {
  work_ = work;

  work_->whenNameChanged(
      [this](const utils::U8String& new_name) {
        NameChanged(new_name);
      },
      shared_from_this());
}

utils::U8String WorkModel::name() const {
  return work_->name();
}

bool WorkModel::set_name(const utils::U8String& new_name) {
  return work_->set_name(new_name);
}

}  // namespace snailcore
