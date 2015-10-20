// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <memory>

#include "utils/basic_utils.h"
#include "snail/i_kbnode_attribute_model.h"
#include "src/qtui/core/kbnode_attribute_edit_presenter.h"
#include "src/qtui/ui/kbnode_attribute_edit_view.h"
#include "src/qtui/core/tree_item_qmodel.h"
#include "pfmvp/pf_view_factory_manager.h"
#include "snail/i_kbnode.h"

using namespace pfmvp;  // NOLINT
using namespace snailcore;  // NOLINT

class KbNodeAttributeEditViewFactory
    : public PfViewFactoryT<IKbNodeAttributeModel> {
 public:
  KbNodeAttributeEditViewFactory() = default;
  virtual ~KbNodeAttributeEditViewFactory() = default;

  DEF_VIEW_FACTORY_ID(KbNodeAttributeEditViewFactory)

  std::shared_ptr<PfPresenter>
  createViewFor(std::shared_ptr<IKbNodeAttributeModel> model,
                PfCreateViewArgs* args) override {
    (void)args;
    auto view = std::make_shared<KbNodeAttributeEditView>();
    auto kbnode_tree_model =
        std::make_shared<TreeItemQModelWithClearAndAddMoreRow<IKbNode>>();
    return std::make_shared<KbNodeAttributeEditPresenter>(
        model, view, std::move(kbnode_tree_model));
  }

 private:
  SNAIL_DISABLE_COPY(KbNodeAttributeEditViewFactory);
};

static view_factory_t<IKbNodeAttributeModel, KbNodeAttributeEditViewFactory>
g_kbnodeattributeedit_view_factory;
