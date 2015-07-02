// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_ATTRIBUTE_ADDER_PRESENTER_H_
#define SRC_QTUI_ATTRIBUTE_ADDER_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_attribute_adder_model.h"
#include "qtui/i_attribute_adder_dialog.h"

using AttributeAdderPresenterBase =
    pfmvp::PfPresenterT<snailcore::IAttributeAdderModel,
                        IAttributeAdderDialog>;

class IAttributeSelectorQModel;
class AttributeAdderPresenterImpl;

class AttributeAdderPresenter : public AttributeAdderPresenterBase {
 public:
  AttributeAdderPresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view,
      std::unique_ptr<IAttributeSelectorQModel> attrListQModel);
  ~AttributeAdderPresenter();

  void initialize() override;

 private:
  AttributeAdderPresenter(const AttributeAdderPresenter&) = delete;
  AttributeAdderPresenter& operator=(const AttributeAdderPresenter&) = delete;

  std::unique_ptr<IAttributeSelectorQModel> attrListQModel_;

  friend class AttributeAdderPresenterImpl;
  std::unique_ptr<AttributeAdderPresenterImpl> impl_;
};

#endif  // SRC_QTUI_ATTRIBUTE_ADDER_PRESENTER_H_
