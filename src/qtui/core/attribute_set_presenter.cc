// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/core/attribute_set_presenter.h"
#include "qtui/core/fto_attribute_set_layout.h"
#include "snail/i_attribute_model.h"
#include "qtui/ui/i_attribute_editor_view.h"

using snailcore::IAttribute;
using snailcore::IAttributeSupplier;

AttributeSetPresenter::AttributeSetPresenter(
    std::shared_ptr<model_type> model, std::shared_ptr<view_type> view,
    std::shared_ptr<fto::AttributeSetLayout> attr_set_layout)
    : AttributeSetPresenterBase(model, view),
      attr_set_layout_(std::move(attr_set_layout)) {}

AttributeSetPresenter::~AttributeSetPresenter() = default;

void AttributeSetPresenter::initialize() {
  resetAttrSuppliers(model()->isEditMode());

  view()->whenUserSwitchMode([this]() { model()->switchMode(); },
                             shared_from_this());

  model()->whenSwitchToEditMode([this]() { resetAttrSuppliers(true); },
                                shared_from_this());

  model()->whenSwitchToDisplayMode([this]() { resetAttrSuppliers(false); },
                                   shared_from_this());

  attr_set_layout_->whenCreateAttrEditor(
      [this](IAttribute* attr) {
        auto attr_model = model()->createAttributeModel(attr);
        return createRawViewFor<IAttributeEditorView>(attr_model);
      },
      shared_from_this());

  attr_set_layout_->whenCloseAttributeEditors(
      [this]() { model()->closeAttributeEditors(triad_manager()); },
      shared_from_this());

  model()->whenValidateComplete(
      [this](bool validate_result) {
        view()->setSwitchModelButtonEnabled(validate_result);
      },
      shared_from_this());
}

void AttributeSetPresenter::resetAttrSuppliers(bool edit_mode) {
  attr_set_layout_->setAttributeSuppliers(model()->getAttributeSuppliers(),
                                          edit_mode);
  if (edit_mode)
    view()->switchToEditMode();
  else
    view()->switchToDisplayMode();
}
