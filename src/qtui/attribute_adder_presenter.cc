// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/attribute_adder_presenter.h"

#include <memory>

#include "utils/basic_utils.h"
#include "qtui/i_attribute_list_qmodel.h"
#include "snail/i_attribute_model.h"
#include "qtui/i_attribute_editor.h"

using namespace snailcore;  // NOLINT

class AttributeAdderPresenterImpl {
 public:
  AttributeAdderPresenterImpl(AttributeAdderPresenter* presenter)
      : presenter_(presenter) { }
  ~AttributeAdderPresenterImpl() = default;

  void buildAttributeEditor(std::shared_ptr<IAttributeModel> attr_model) {
    auto attr_editor =
        presenter_->createRawViewFor<IAttributeEditor>(attr_model);
    presenter_->view()->setAttributeEditor(attr_editor);
    presenter_->view()->setAddButtonEnabled(false);
  }

 private:
  AttributeAdderPresenterImpl(const AttributeAdderPresenterImpl&) = delete;
  AttributeAdderPresenterImpl& operator=(
      const AttributeAdderPresenterImpl&) = delete;

  friend class AttributeAdderPresenter;
  AttributeAdderPresenter* presenter_ { nullptr };
};


AttributeAdderPresenter::AttributeAdderPresenter(
    std::shared_ptr<IAttributeAdderModel> model,
    std::shared_ptr<IAttributeAdderDialog> view,
    std::unique_ptr<IAttributeListQModel> attrListQModel)
    : AttributeAdderPresenterBase(model, view)
    , attrListQModel_(std::move(attrListQModel))
    , impl_(utils::make_unique<AttributeAdderPresenterImpl>(this)) {
}

AttributeAdderPresenter::~AttributeAdderPresenter() = default;

void AttributeAdderPresenter::initialize() {
  view()->setPrompt(model()->getPrompt());

  attrListQModel_->setAttributeList(model()->getAllowedAttributeList());
  view()->setAttributeListQModel(attrListQModel_.get());

  view()->setCurrentAttributeIndex(model()->getCurrentAttributeIndex());

  impl_->buildAttributeEditor(model()->getCurrentAttributeModel());

  view()->setDoneButtonEnabled(true);

  view()->whenCurrentAttributeIndexChanged(
      [this](int index) {
        model()->setCurrentAttributeIndex(index);
      },
      shared_from_this());

  view()->whenAddButtonClicked(
      [this]() {
        model()->doAddAttribute();
      },
      shared_from_this());

  model()->whenValidateComplete(
      [this](bool result) {
        view()->setAddButtonEnabled(result);
      },
      shared_from_this());

  model()->whenDiscardAttributeModel(
      [this](IAttributeModel* model) {
        auto attr_editor = findSingleViewByModel<IAttributeEditor>(model);
        view()->removeAttributeEditor(attr_editor);
        removeTriadBy(model);
      },
      shared_from_this());

  model()->whenCurrentAttributeModelChanged(
      [this](std::shared_ptr<IAttributeModel> attr_model) {
        impl_->buildAttributeEditor(attr_model);
      },
      shared_from_this());
}
