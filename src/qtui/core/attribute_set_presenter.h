// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CORE_ATTRIBUTE_SET_PRESENTER_H_
#define SRC_QTUI_CORE_ATTRIBUTE_SET_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_attribute_set_model.h"
#include "qtui/i_attribute_set_view.h"

using AttributeSetPresenterBase =
    pfmvp::PfPresenterT<snailcore::IAttributeSetModel,
                        IAttributeSetView>;

class IAttributeSetLayout;

class AttributeSetPresenter : public AttributeSetPresenterBase {
 public:
  AttributeSetPresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view,
      std::shared_ptr<IAttributeSetLayout> attr_set_layout);
  virtual ~AttributeSetPresenter();

  void initialize() override;

 private:
  SNAIL_DISABLE_COPY(AttributeSetPresenter);

  void resetAttrSuppliers(bool edit_mode);

  std::shared_ptr<IAttributeSetLayout> attr_set_layout_;
};

#endif  // SRC_QTUI_CORE_ATTRIBUTE_SET_PRESENTER_H_
