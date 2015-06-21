// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/workspace_presenter.h"

using namespace snailcore;  // NOLINT
using namespace pfmvp;  // NOLINT

void WorkSpacePresenter::initialize() {
  view()->whenUserClickAddWork(
      [this](const utils::U8String& work_name) {
        model()->createWork(work_name);
      },
      shared_from_this());

  view()->whenUserCloseWork(
      [this](IWorkView* work_view) {
        removeTriadBy(work_view);
      },
      shared_from_this());

  model()->whenWorkModelAdded(
      [this](std::shared_ptr<IWorkModel> work_model) {
        onWorkModelAdded(work_model);
      },
      shared_from_this());

  model()->whenActiveWorkModelChanged(
      [this](IWorkModel* work_model) {
        auto work_view = findSingleViewByModel<IWorkView>(work_model);
        if (work_view) {
          view()->setActiveWorkView(work_view);
        }
      },
      shared_from_this());

  model()->whenWorkModelActivelyRemoved(
      [this](IWorkModel* work_model) {
        removeTriadBy(work_model);
      },
      shared_from_this());
}

void WorkSpacePresenter::onAboutToDestroyModel(IPfModel* pfmodel) {
  auto work_model = dynamic_cast<IWorkModel*>(pfmodel);
  if (work_model)
    model()->removeWorkModel(work_model);
}

void WorkSpacePresenter::onAboutToDestroyView(IPfView* pfview) {
  auto work_view = dynamic_cast<IWorkView*>(pfview);
  if (work_view)
    view()->removeWorkView(work_view);
}

void WorkSpacePresenter::onWorkModelAdded(
    std::shared_ptr<IWorkModel> work_model) {
  auto work_view = createRawViewFor<IWorkView>(work_model);
  if (work_view) {
    view()->addWorkView(work_view, work_model->name());
    view()->setActiveWorkView(work_view);
    monitorModelDestroy(work_model.get());
    monitorViewDestroy(work_view);

    auto raw_work_model = work_model.get();
    work_model->whenBasicInfoChanged(
        [this, raw_work_model, work_view]() {
          view()->updateWorkViewTitle(work_view, raw_work_model->name());
        },
        shared_from_this());
  }
}
