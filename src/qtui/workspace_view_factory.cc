// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "snail/i_workspace_model.h"
#include "src/qtui/workspace_presenter.h"
#include "src/qtui/workspace_view.h"
#include "pfmvp/pf_view_factory_manager.h"

using namespace pfmvp;  // NOLINT
using namespace snailcore;  // NOLINT

class WorkSpaceViewFactory : public IPfViewFactory {
 public:
  WorkSpaceViewFactory() = default;
  virtual ~WorkSpaceViewFactory() = default;

  DEF_VIEW_FACTORY_ID(WorkSpaceViewFactory)

  std::shared_ptr<PfPresenter>
  createView(std::shared_ptr<IPfModel> model) override {
    auto the_model = std::dynamic_pointer_cast<IWorkSpaceModel>(model);
    if (the_model) {
      auto view = std::make_shared<WorkSpaceView>();
      return WorkSpacePresenter::create(the_model, view);
    }

    return nullptr;
  }

 private:
  WorkSpaceViewFactory(const WorkSpaceViewFactory& other) = delete;
  WorkSpaceViewFactory& operator=(const WorkSpaceViewFactory& other) = delete;
};

static view_factory_t<IWorkSpaceModel, WorkSpaceViewFactory>
g_WorkSpace_view_factory;
