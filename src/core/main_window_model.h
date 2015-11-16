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
#include "utils/and_slot_bool_result_combiner.h"

namespace snailcore {

class IWorkSpaceModel;

class MainWindowModelSignalHelper;

class MainWindowModel : public IMainWindowModel {
 public:
  explicit MainWindowModel(std::shared_ptr<IWorkSpaceModel> workspace_model);
  ~MainWindowModel();

  const utils::U8String& windowTitle() const override;
  void setWindowTitle(const utils::U8String& newTitle) override;

  std::shared_ptr<IWorkSpaceModel> getWorkSpaceModel() override;
  void createWork(const utils::U8String& work_name) override;

  bool requestClose() const override;

 public:
  SNAIL_SIGSLOT_OVERRIDE(WindowTitleChanged)
  SNAIL_SIGSLOT_OVERRIDE(RequestClose);

 private:
  MainWindowModel(const MainWindowModel& other) = delete;
  MainWindowModel& operator=(const MainWindowModel& other) = delete;

  std::unique_ptr<MainWindowModelSignalHelper> signal_helper_;

  utils::U8String windowTitle_;
  std::shared_ptr<IWorkSpaceModel> workspace_model_;
};

}  // namespace snailcore

#endif  // SRC_CORE_MAIN_WINDOW_MODEL_H_
