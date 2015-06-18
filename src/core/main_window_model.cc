// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include "src/core/main_window_model.h"

#include "utils/basic_utils.h"
#include "utils/i18n.h"

#include "src/core/workspace_model.h"
#include "src/core/work_model_factory.h"

namespace snailcore {

std::shared_ptr<IMainWindowModel> makeMainWindowModel() {
  static std::weak_ptr<IMainWindowModel> cache;

  auto model = cache.lock();
  if (!model) {
    // TODO(lutts): who will delete work_model_factory?or do not need to delete?
    auto work_model_factory = new WorkModelFactory();
    auto workspace_model = std::make_shared<WorkSpaceModel>(work_model_factory);
    model = std::make_shared<MainWindowModel>(workspace_model);
    cache = model;
  }

  return model;
}

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

bool MainWindowModel::requestClose() const {
  return RequestClose();
}

}  // namespace snailcore
