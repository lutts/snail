// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_WORKSPACE_PRESENTER_H_
#define SRC_QTUI_WORKSPACE_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_workspace_model.h"
#include "qtui/i_workspace_view.h"
#include "snail/i_work_model.h"
#include "qtui/i_work_view.h"

using WorkSpacePresenterBase =
    pfmvp::PfPresenterT<snailcore::IWorkSpaceModel,
                        IWorkSpaceView>;

class WorkSpacePresenter : public WorkSpacePresenterBase {
 public:
  WorkSpacePresenter(std::shared_ptr<snailcore::IWorkSpaceModel> model,
                std::shared_ptr<IWorkSpaceView> view)
      : WorkSpacePresenterBase(model, view) {
  }

  void initialize() override;

 private:
  WorkSpacePresenter(const WorkSpacePresenter& other) = delete;
  WorkSpacePresenter& operator=(const WorkSpacePresenter& other) = delete;

  void onAboutToDestroyModel(pfmvp::IPfModel* pfmodel) override;
  void onAboutToDestroyView(pfmvp::IPfView* pfview) override;

  void onWorkModelAdded(std::shared_ptr<snailcore::IWorkModel> work_model);
};

#endif  // SRC_QTUI_WORKSPACE_PRESENTER_H_