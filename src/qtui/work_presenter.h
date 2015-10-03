// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_WORK_PRESENTER_H_
#define SRC_QTUI_WORK_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_work_model.h"
#include "qtui/i_work_view.h"

using WorkPresenterBase =
    pfmvp::PfPresenterT<snailcore::IWorkModel,
                        IWorkView>;

class WorkPresenter : public WorkPresenterBase {
 public:
  WorkPresenter(std::shared_ptr<model_type> model,
                std::shared_ptr<view_type> view)
      : WorkPresenterBase(model, view) {
  }

  void initialize() override;

 private:
  WorkPresenter(const WorkPresenter& other) = delete;
  WorkPresenter& operator=(const WorkPresenter& other) = delete;
};

#endif  // SRC_QTUI_WORK_PRESENTER_H_
