// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QString>

#include "src/qtui/core/work_presenter.h"
#include "snail/i_attribute_set_model.h"
#include "qtui/i_attribute_set_view.h"

void WorkPresenter::initialize() {
  auto work_name = U8StringToQString(model()->name());
  view()->setWorkName(work_name);

  auto attr_set_view =
      createRawViewFor<IAttributeSetView>(
          model()->createAttributeSetModel());
  view()->setWorkAttrSetView(attr_set_view);

  view()->whenUserSetWorkName(
      [this](const QString& new_name) {
        model()->set_name(QStringToU8String(new_name));
      },
      shared_from_this());
}
