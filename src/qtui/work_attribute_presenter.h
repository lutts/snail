// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_WORK_ATTRIBUTE_PRESENTER_H_
#define SRC_QTUI_WORK_ATTRIBUTE_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_work_attribute_model.h"
#include "qtui/i_work_attribute_view.h"
#include "snail/attribute_display_block.h"

using WorkAttributePresenterBase =
    pfmvp::PfPresenterT<snailcore::IWorkAttributeModel,
                        IWorkAttributeView>;

class WorkAttributePresenter : public WorkAttributePresenterBase
                             , public snailcore::IAttributeDisplayBlockVisitor {
 public:
  WorkAttributePresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view,
      IAttributeLayout* attr_layout)
      : WorkAttributePresenterBase(model, view)
      , attr_layout_(attr_layout) {
  }

  void initialize() override;

  // IAttributeDisplayBlockVisitor impl
  void beginTraverse(int total_block_count) override;
  void* visitAttributeGroupDisplayBlock(
      snailcore::AttributeGroupDisplayBlock attr_group_block) override;
  void* visitAttributeDisplayBlock(
      snailcore::AttributeDisplayBlock attr_block) override;
  void endTraverse() override;

 private:
  WorkAttributePresenter(const WorkAttributePresenter&) = delete;
  WorkAttributePresenter& operator=(const WorkAttributePresenter&) = delete;

  IAttributeLayout* attr_layout_;
};

#endif  // SRC_QTUI_WORK_ATTRIBUTE_PRESENTER_H_
