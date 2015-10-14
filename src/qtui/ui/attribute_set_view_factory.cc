// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QWidget>
#include <QGridLayout>

#include <memory>

#include "utils/basic_utils.h"
#include "snail/i_attribute_set_model.h"
#include "src/qtui/core/attribute_set_presenter.h"
#include "src/qtui/ui/attribute_set_view.h"
#include "pfmvp/pf_view_factory_manager.h"
#include "src/qtui/core/attribute_set_layout.h"

using namespace pfmvp;  // NOLINT
using namespace snailcore;  // NOLINT

class AttributeSetViewForWorkViewFactory
    : public PfViewFactoryT<IAttributeSetModel> {
 public:
  AttributeSetViewForWorkViewFactory() = default;
  virtual ~AttributeSetViewForWorkViewFactory() = default;

  DEF_VIEW_FACTORY_ID(AttributeSetViewForWorkViewFactory)

  std::shared_ptr<PfPresenter>
  createViewFor(std::shared_ptr<IAttributeSetModel> model,
                PfCreateViewArgs* args) override {
    (void)args;
    auto view = std::make_shared<AttributeSetViewForWorkView>();
    auto layout = std::make_shared<AttributeSetLayout>();
    view->getAttributeSetContainer()->setLayout(layout->layout());
    return std::make_shared<AttributeSetPresenter>(model, view, layout);
  }

 private:
  SNAIL_DISABLE_COPY(AttributeSetViewForWorkViewFactory);
};

static view_factory_t<IAttributeSetModel, AttributeSetViewForWorkViewFactory>
g_attributeset_view_factory;
