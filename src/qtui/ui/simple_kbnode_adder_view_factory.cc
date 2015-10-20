// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <memory>

#include "utils/basic_utils.h"
#include "snail/i_simple_kbnode_adder_model.h"
#include "src/qtui/core/simple_kbnode_adder_presenter.h"
#include "src/qtui/ui/simple_kbnode_adder_view.h"
#include "pfmvp/pf_view_factory_manager.h"
#include "snail/i_kbnode.h"
#include "src/qtui/core/tree_item_qmodel.h"

using namespace pfmvp;  // NOLINT
using namespace snailcore;  // NOLINT

class SimpleKbNodeAdderViewFactory : public PfViewFactoryT<ISimpleKbNodeAdderModel> {
 public:
  SimpleKbNodeAdderViewFactory() = default;
  virtual ~SimpleKbNodeAdderViewFactory() = default;

  DEF_VIEW_FACTORY_ID(SimpleKbNodeAdderViewFactory)

  std::shared_ptr<PfPresenter>
  createViewFor(std::shared_ptr<ISimpleKbNodeAdderModel> model,
                PfCreateViewArgs* args) override {
    (void)args;
    auto view = std::make_shared<SimpleKbNodeAdderView>();
    return std::make_shared<SimpleKbNodeAdderPresenter>(
        model, view, utils::make_unique<TreeItemQModelWithProviderRoot<IKbNode>>());
  }

 private:
  SNAIL_DISABLE_COPY(SimpleKbNodeAdderViewFactory);
};

static view_factory_t<ISimpleKbNodeAdderModel, SimpleKbNodeAdderViewFactory>
g_kbnodeprovider_view_factory;
