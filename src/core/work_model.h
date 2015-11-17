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

namespace snailcore {

class IAttributeSetModelFactory;

class WorkModelSignalHelper;

class WorkModel : public IWorkModel,
                  public utils::ITrackable,
                  public std::enable_shared_from_this<WorkModel> {
 public:
  explicit WorkModel(IAttributeSetModelFactory* attr_set_model_factory);
  virtual ~WorkModel();

  void set_work(fto::Work* work) override;

  utils::U8String name() const override;
  bool set_name(const utils::U8String& new_name) override;

  std::shared_ptr<IAttributeSetModel> createAttributeSetModel() override;

 public:
  SNAIL_SIGSLOT_OVERRIDE(NameChanged);

 private:
  WorkModel(const WorkModel& other) = delete;
  WorkModel& operator=(const WorkModel& other) = delete;

  std::unique_ptr<WorkModelSignalHelper> signal_helper_;
  fto::Work* work_{nullptr};
  IAttributeSetModelFactory* attr_set_model_factory_;
};

}  // namespace snailcore

#endif  // SRC_CORE_WORK_MODEL_H_
