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

#include "snail/i_attribute_adder_model.h"

class IWorkBasicInfoQModel;

using WorkPresenterBase =
    pfmvp::PfPresenterT<snailcore::IWorkModel,
                        IWorkView>;

class WorkPresenter : public WorkPresenterBase {
 public:
  WorkPresenter(std::shared_ptr<snailcore::IWorkModel> model,
                std::shared_ptr<IWorkView> view,
                std::unique_ptr<IWorkBasicInfoQModel> basicInfoQModel)
      : WorkPresenterBase(model, view)
      , basicInfoQModel_(std::move(basicInfoQModel)) {
  }

  void initialize() override {
    view()->setBasicInfoQModel(basicInfoQModel_.get());

    view()->whenUserClickAddAttribute(
        [this]() {
          showDialog(model()->createAttributeAdderModel());
        },
        shared_from_this());
  }

 private:
  WorkPresenter(const WorkPresenter& other) = delete;
  WorkPresenter& operator=(const WorkPresenter& other) = delete;

  std::unique_ptr<IWorkBasicInfoQModel> basicInfoQModel_;
};

#endif  // SRC_QTUI_WORK_PRESENTER_H_
