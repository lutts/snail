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

class MainWindowModel : public IMainWindowModel {
 public:
  MainWindowModel();
  ~MainWindowModel();

  const utils::U8String& windowTitle() const override;
  void setWindowTitle(const utils::U8String& newTitle) override;
  bool requestClose() const override;

 private:
  MainWindowModel(const MainWindowModel& other) = delete;
  MainWindowModel& operator=(const MainWindowModel& other) = delete;

  utils::U8String windowTitle_;

 private:
  SNAIL_SIGSLOT_IMPL(WindowTitleChanged)
  SNAIL_SIGSLOT_COMBINER_IMPL(RequestClose, and_slot_bool_result_combiner)
};

std::shared_ptr<IMainWindowModel> makeMainWindowModel();

}  // namespace snailcore

#endif  // SRC_CORE_MAIN_WINDOW_MODEL_H_
