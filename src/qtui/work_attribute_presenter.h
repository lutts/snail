// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef WORK_ATTRIBUTE_PRESENTER_H_
#define WORK_ATTRIBUTE_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_work_attribute_model.h"
#include "qtui/i_work_attribute_view.h"

using WorkAttributePresenterBase =
    pfmvp::PfPresenterT<snailcore::IWorkAttributeModel,
                        IWorkAttributeView>;

class WorkAttributePresenter : public WorkAttributePresenterBase {
 public:
  WorkAttributePresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view)
      : WorkAttributePresenterBase(model, view) {
  }

  void initialize() override;

 private:
  WorkAttributePresenter(const WorkAttributePresenter&) = delete;
  WorkAttributePresenter& operator=(const WorkAttributePresenter&) = delete;

  void reLayoutUI();
  void showAttributes();
  void showCommands();
};

#endif  // WORK_ATTRIBUTE_PRESENTER_H_
