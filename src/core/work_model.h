// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_WORK_MODEL_H_
#define SRC_CORE_WORK_MODEL_H_

#include <memory>

#include "utils/i_trackable.h"
#include "snail/i_work_model.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class WorkModel : public IWorkModel
                , public utils::ITrackable
                , public std::enable_shared_from_this<WorkModel> {
 public:
  WorkModel() = default;
  virtual ~WorkModel();

  void set_work(IWork* work);

  utils::U8String name() const override;
  bool set_name(const utils::U8String& new_name) override;

  // TODO(lutts): impl this
  std::shared_ptr<IAttributeAdderModel>
  createAttributeAdderModel() const { return nullptr; }

 private:
  WorkModel(const WorkModel& other) = delete;
  WorkModel& operator=(const WorkModel& other) = delete;

  IWork* work_ { nullptr };

 private:
  SNAIL_SIGSLOT_IMPL(NameChanged);
};

}  // namespace snailcore

#endif  // SRC_CORE_WORK_MODEL_H_
