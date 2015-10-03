// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QString>

#include "src/qtui/core/work_presenter.h"

void WorkPresenter::initialize() {
  auto work_name = U8StringToQString(model()->name());
  view()->setWorkName(work_name);

  view()->whenUserSetWorkName(
      [this](const QString& new_name) {
        model()->set_name(QStringToU8String(new_name));
      },
      shared_from_this());
}
