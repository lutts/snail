// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/attribute_adder_presenter.h"

#include <memory>

#include "utils/basic_utils.h"
#include "qtui/i_attribute_selector_qmodel.h"
#include "snail/i_attribute_editor_model.h"
#include "qtui/i_attribute_editor_view.h"
#include "qtui/i_candidate_item_qmodel_adapter.h"

using namespace snailcore;  // NOLINT

class AttributeAdderPresenterImpl {
 public:
  explicit AttributeAdderPresenterImpl(AttributeAdderPresenter* presenter)
      : presenter_(presenter) { }
  ~AttributeAdderPresenterImpl() = default;

  void buildAttributeEditorView(
      std::shared_ptr<IAttributeEditorModel> attr_model) {
    auto attr_editor =
        presenter_->createRawViewFor<IAttributeEditorView>(attr_model);

    auto view = presenter_->view();
    auto model = presenter_->model();

    view->setAttributeEditor(attr_editor);
    view->setAddButtonEnabled(model->validateResult());
  }

 private:
  AttributeAdderPresenterImpl(const AttributeAdderPresenterImpl&) = delete;
  AttributeAdderPresenterImpl& operator=(
      const AttributeAdderPresenterImpl&) = delete;

  friend class AttributeAdderPresenter;
  AttributeAdderPresenter* presenter_ { nullptr };
};


AttributeAdderPresenter::AttributeAdderPresenter(
    std::shared_ptr<model_type> model,
    std::shared_ptr<view_type> view,
    std::unique_ptr<ICandidateItemQModelAdapter> attr_candidate_adapter)
    : AttributeAdderPresenterBase(model, view)
    , attr_candidate_adapter_(std::move(attr_candidate_adapter))
    , impl_(utils::make_unique<AttributeAdderPresenterImpl>(this)) {
}

AttributeAdderPresenter::~AttributeAdderPresenter() = default;

void AttributeAdderPresenter::initialize() {
  view()->setPrompt(model()->getPrompt());

  auto attr_candidates_root = model()->getAllowedAttributes();
  attr_candidate_adapter_->setCandidates(*attr_candidates_root);
  view()->setAttributeSelectorQModel(attr_candidate_adapter_.get());

  view()->setCurrentAttributeName(model()->getCurrentAttributeName());

  impl_->buildAttributeEditorView(model()->getCurrentAttributeEditorModel());

  view()->setDoneButtonEnabled(true);

  view()->whenCurrentAttributeChanged(
      [this](void* item_ptr) {
        auto item = static_cast<CandidateItem*>(item_ptr);
        model()->setCurrentAttribute(*item);
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

  model()->whenDiscardAttributeEditorModel(
      [this](IAttributeEditorModel* model) {
        auto attr_editor = findSingleViewByModel<IAttributeEditorView>(model);
        view()->removeAttributeEditor(attr_editor);
        removeTriadBy(model);
      },
      shared_from_this());

  model()->whenCurrentAttributeEditorModelChanged(
      [this](std::shared_ptr<IAttributeEditorModel> attr_model) {
        impl_->buildAttributeEditorView(attr_model);
      },
      shared_from_this());
}
