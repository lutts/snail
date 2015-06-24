// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef ATTRIBUTE_ADDER_PRESENTER_H_
#define ATTRIBUTE_ADDER_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_attribute_adder_model.h"
#include "qtui/i_attribute_adder_dialog.h"

using AttributeAdderPresenterBase =
    pfmvp::PfPresenterT<snailcore::IAttributeAdderModel,
                        IAttributeAdderDialog>;

class IAttributeListQModel;

class AttributeAdderPresenterImpl;

class AttributeAdderPresenter : public AttributeAdderPresenterBase {
 public:
  AttributeAdderPresenter(
      std::shared_ptr<snailcore::IAttributeAdderModel> model,
      std::shared_ptr<IAttributeAdderDialog> view,
      std::unique_ptr<IAttributeListQModel> attrListQModel);
  ~AttributeAdderPresenter();

  void initialize() override;

 private:
  AttributeAdderPresenter(const AttributeAdderPresenter& other) = delete;
  AttributeAdderPresenter& operator=(const AttributeAdderPresenter& other) = delete;

  std::unique_ptr<IAttributeListQModel> attrListQModel_;

  friend class AttributeAdderPresenterImpl;
  std::unique_ptr<AttributeAdderPresenterImpl> impl_;
};

#endif  // ATTRIBUTE_ADDER_PRESENTER_H_
