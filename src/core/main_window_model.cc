// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include "src/core/main_window_model.h"

#include "utils/basic_utils.h"
#include "utils/i18n.h"

namespace snailcore {

std::shared_ptr<IMainWindowModel> makeMainWindowModel() {
  static std::weak_ptr<IMainWindowModel> cache;

  auto model = cache.lock();
  if (!model) {
    model = std::make_shared<MainWindowModel>();
    cache = model;
  }

  return model;
}

MainWindowModel::MainWindowModel()
    : windowTitle_(_("Snail")) {
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

bool MainWindowModel::requestClose() const {
  return RequestClose();
}

}  // namespace snailcore
