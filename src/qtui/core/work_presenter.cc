// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QString>

#include "src/qtui/core/work_presenter.h"
#include "pfmvp/i_pf_create_view_args.h"
#include "snail/i_attribute_set_model.h"
#include "qtui/ui/i_attribute_set_view.h"

void WorkPresenter::initialize() {
  auto work_name = U8StringToQString(model()->name());
  view()->setWorkName(work_name);

  pfmvp::PfCreateViewArgs args;
  args.set_view_factory_id(PF_VIEW_FACTORY(AttributeSetViewForWorkViewFactory));
  auto attr_set_view = createRawViewFor<IAttributeSetView>(
      model()->createAttributeSetModel(), &args);
  view()->setWorkAttrSetView(attr_set_view);

  view()->whenUserSetWorkName([this](const QString& new_name) {
                                model()->set_name(QStringToU8String(new_name));
                              },
                              shared_from_this());
}
