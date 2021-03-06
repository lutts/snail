// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <memory>

#include "utils/basic_utils.h"  // utils::make_unique
#include "snail/i_work_model.h"
#include "src/qtui/core/work_presenter.h"
#include "src/qtui/ui/work_view.h"
#include "pfmvp/pf_view_factory_manager.h"

using namespace pfmvp;      // NOLINT
using namespace snailcore;  // NOLINT

class WorkViewFactory : public PfViewFactoryT<IWorkModel> {
 public:
  WorkViewFactory() = default;
  virtual ~WorkViewFactory() = default;

  DEF_VIEW_FACTORY_ID(WorkViewFactory)

  std::shared_ptr<PfPresenter> createViewFor(std::shared_ptr<IWorkModel> model,
                                             PfCreateViewArgs* args) override {
    V_UNUSED(args);
    auto view = std::make_shared<WorkView>();
    return std::make_shared<WorkPresenter>(model, view);
  }

 private:
  WorkViewFactory(const WorkViewFactory& other) = delete;
  WorkViewFactory& operator=(const WorkViewFactory& other) = delete;
};

static view_factory_t<IWorkModel, WorkViewFactory> g_Work_view_factory;
