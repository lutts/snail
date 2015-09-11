// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/attribute_collection_presenter.h"

#include "qtui/i_attribute_collection_qmodel.h"
#include "qtui/i_attribute_delegate.h"
#include "snail/i_attribute_model.h"
#include "qtui/i_attribute_editor_view.h"

AttributeCollectionPresenter::AttributeCollectionPresenter(
    std::shared_ptr<model_type> model,
    std::shared_ptr<view_type> view,
    std::shared_ptr<IAttributeCollectionQModel> qmodel,
    std::unique_ptr<IAttributeDelegate> attr_delegate)
    : AttributeCollectionPresenterBase(model, view)
    , qmodel_(std::move(qmodel))
    , attr_delegate_(std::move(attr_delegate)) { }

AttributeCollectionPresenter::~AttributeCollectionPresenter() = default;

void AttributeCollectionPresenter::initialize() {
  qmodel_->setAttributeSuppliers(model()->getAttributeSuppliers());
  view()->setQModel(qmodel_.get());

  attr_delegate_->whenCreateEditorFor(
      [this](IAttribute* attr) {
        auto attr_model = model()->createAttributeModel(attr);
        return createRawViewFor<IAttributeEditorView>(attr_model);
      },
      shared_from_this());

  attr_delegate_->whenCloseEditor(
      [this](IAttributeEditorView* attr_editor_view) {
        removeTriadBy(attr_editor_view);
      },
      shared_from_this());

  view()->setAttributeDelegate(attr_delegate_.get());

  view()->whenSwitchToEditMode(
      [this]() {
        qmodel_->aboutToSwitchMode();
        model()->switchToEditMode();
        qmodel_->switchToEditMode();

        int row_count = qmodel_->attrRowCount();
        for (int row = 0; row < row_count; ++row) {
          view()->openAttributeEditor(row);
        }
      },
      shared_from_this());

  view()->whenSwitchToDisplayMode(
      [this]() {
        int row_count = qmodel_->attrRowCount();
        for (int row = 0; row < row_count; ++row) {
          view()->closeAttributeEditor(row);
        }

        qmodel_->aboutToSwitchMode();
        model()->switchToDisplayMode();
        qmodel_->switchToDisplayMode();
      },
      shared_from_this());

  view()->whenUserMayClickAddAttribute(
      [this](int row) {
        qmodel_->mayAddAttributeIfSupplier(row);
      },
      shared_from_this());

  qmodel_->whenAttributeAdded(
      [this](int row) {
        view()->openAttributeEditor(row);
      },
      shared_from_this());
}
