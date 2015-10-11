// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <memory>

#include "utils/basic_utils.h"
#include "snail/i_kbnode_provider_model.h"
#include "src/qtui/core/kbnode_provider_presenter.h"
#include "src/qtui/ui/kbnode_provider_view.h"
#include "pfmvp/pf_view_factory_manager.h"
#include "src/qtui/core/kbnode_tree_qmodel.h"

using namespace pfmvp;  // NOLINT
using namespace snailcore;  // NOLINT

class KbNodeProviderViewFactory : public PfViewFactoryT<IKbNodeProviderModel> {
 public:
  KbNodeProviderViewFactory() = default;
  virtual ~KbNodeProviderViewFactory() = default;

  DEF_VIEW_FACTORY_ID(KbNodeProviderViewFactory)

  std::shared_ptr<PfPresenter>
  createViewFor(std::shared_ptr<IKbNodeProviderModel> model,
                PfCreateViewArgs* args) override {
    (void)args;
    auto view = std::make_shared<KbNodeProviderView>();
    return std::make_shared<KbNodeProviderPresenter>(
        model, view, utils::make_unique<KbNodeTreeQModelWithProviderNode>());
  }

 private:
  SNAIL_DISABLE_COPY(KbNodeProviderViewFactory);
};

static view_factory_t<IKbNodeProviderModel, KbNodeProviderViewFactory>
g_kbnodeprovider_view_factory;
