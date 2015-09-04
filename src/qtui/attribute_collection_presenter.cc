// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/attribute_collection_presenter.h"
#include "src/qtui/attr_create_view_args.h"
#include "snail/i_attribute_model.h"
#include "qtui/i_attribute_view.h"

using namespace snailcore;  // NOLINT

void AttributeCollectionPresenter::initialize() {
  view()->setEditMode(model()->isEditMode());
  attr_layout_->set_triad_manager(triad_manager());

  model()->traverseAttributes(this);

  view()->whenEditModeButtonClicked(
      [this]() {
        model()->switchToEditMode();
      },
      shared_from_this());

  view()->whenDoneButtonClicked(
      [this]() {
        model()->switchToDisplayMode();
      },
      shared_from_this());

  model()->whenAttributesChanged(
      [this]() {
        // TODO(lutts): do we need to setEditMode to view?
        model()->traverseAttributes(this);
      },
      shared_from_this());

  model()->whenAttrLabelChanged(
      [this](UpdateAttrLabelData label_data) {
        attr_layout_->updateLabel(label_data);
      },
      shared_from_this());
}

//////////////////// IAttributeDisplayBlockVisitor impl begin /////////////
void AttributeCollectionPresenter::beginTraverse(
    int total_block_count) {
  attr_layout_->beginLayout(total_block_count);
}

void* AttributeCollectionPresenter::visitAttributeGroupDisplayBlock(
    AttributeGroupDisplayBlock attr_group_block) {
  return attr_layout_->layoutAttributeGroupDisplayBlock(attr_group_block);
}

void* AttributeCollectionPresenter::visitAttributeDisplayBlock(
    AttributeDisplayBlock attr_block) {
  auto args = AttrCreateViewArgs::getArgs(attr_block.edit_mode);
  auto attr_view =
      createRawViewIfNotExist<IAttributeView>(attr_block.attr_model, args);
  if (attr_view) {
    AttributeViewDisplayBlock attr_view_block;
    attr_view_block.label = attr_block.label;
    attr_view_block.attr_view = attr_view;
    attr_view_block.is_in_group = attr_block.is_in_group;
    attr_view_block.view_priv_data = attr_block.view_priv_data;

    return attr_layout_->layoutAttributeDisplayBlock(attr_view_block);
  }

  return nullptr;
}

#if 0
void AttributeCollectionPresenter::updateLabel(UpdateAttrLabelData label_data) {
}
#endif

void AttributeCollectionPresenter::endTraverse(bool remove_triads) {
  attr_layout_->endLayout(remove_triads);
}
///////////////////// IAttributeDisplayBlockVisitor impl end ///////////////
