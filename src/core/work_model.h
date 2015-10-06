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

class IAttributeCollectionModelFactory;

class WorkModel : public IWorkModel
                , public utils::ITrackable
                , public std::enable_shared_from_this<WorkModel> {
 public:
  WorkModel(
      IAttributeCollectionModelFactory* attr_collection_model_factory);
  virtual ~WorkModel();

  void set_work(IWork* work) override;

  utils::U8String name() const override;
  bool set_name(const utils::U8String& new_name) override;

  std::shared_ptr<IAttributeCollectionModel>
  createAttributeCollectionModel() override;

 private:
  WorkModel(const WorkModel& other) = delete;
  WorkModel& operator=(const WorkModel& other) = delete;

  IWork* work_ { nullptr };
  IAttributeCollectionModelFactory* attr_collection_model_factory_;

 private:
  SNAIL_SIGSLOT_IMPL(NameChanged);
};

}  // namespace snailcore

#endif  // SRC_CORE_WORK_MODEL_H_
