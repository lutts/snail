// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_MAIN_WINDOW_MODEL_H_
#define SRC_CORE_MAIN_WINDOW_MODEL_H_

#include <memory>  // std::shared_ptr;

#include "snail/i_main_window_model.h"
#include "utils/signal_slot.h"
#include "utils/signal_slot_impl.h"
#include "utils/and_slot_bool_result_combiner.h"

namespace snailcore {

class IWorkSpaceModel;

class MainWindowModel : public IMainWindowModel {
 public:
  explicit MainWindowModel(std::shared_ptr<IWorkSpaceModel> workspace_model);
  ~MainWindowModel();

  const utils::U8String& windowTitle() const override;
  void setWindowTitle(const utils::U8String& newTitle) override;

  std::shared_ptr<IWorkSpaceModel> getWorkSpaceModel() override;
  void createWork(const utils::U8String& work_name) override;

  bool requestClose() const override;

 private:
  MainWindowModel(const MainWindowModel& other) = delete;
  MainWindowModel& operator=(const MainWindowModel& other) = delete;

  utils::U8String windowTitle_;
  std::shared_ptr<IWorkSpaceModel> workspace_model_;

 private:
  SNAIL_SIGSLOT_OVERRIDE(WindowTitleChanged)
  SNAIL_SIGSLOT_COMBINER_OVERRIDE(RequestClose, and_slot_bool_result_combiner)
};

}  // namespace snailcore

#endif  // SRC_CORE_MAIN_WINDOW_MODEL_H_
