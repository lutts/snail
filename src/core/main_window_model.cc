// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include "src/core/main_window_model.h"

#include "utils/basic_utils.h"
#include "utils/i18n.h"

#include "snail/i_workspace_model.h"

namespace snailcore {

MainWindowModel::MainWindowModel(
    std::shared_ptr<IWorkSpaceModel> workspace_model)
    : windowTitle_(_("Snail"))
    , workspace_model_(workspace_model) {
}

MainWindowModel::~MainWindowModel() = default;

const utils::U8String& MainWindowModel::windowTitle() const {
  // TODO(lutts): LOCK
  return windowTitle_;
}

void MainWindowModel::setWindowTitle(const utils::U8String& newTitle) {
  auto oldTitle = windowTitle_;

  windowTitle_ = newTitle;

  if (oldTitle != windowTitle_)
    WindowTitleChanged(windowTitle_);
}

std::shared_ptr<IWorkSpaceModel> MainWindowModel::getWorkSpaceModel() {
  return workspace_model_;
}

void MainWindowModel::createWork(const utils::U8String& work_name) {
  workspace_model_->createWork(work_name);
}

bool MainWindowModel::requestClose() const {
  return RequestClose();
}

}  // namespace snailcore
