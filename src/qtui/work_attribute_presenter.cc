// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "work_attribute_presenter.h"
#include "src/qtui/attr_create_view_args.h"
#include "snail/i_attribute_model.h"
#include "qtui/i_attribute_view.h"

using namespace snailcore;  // NOLINT

void WorkAttributePresenter::initialize() {
  reLayoutUI();

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
        reLayoutUI();
      },
      shared_from_this());

  model()->whenShowPopupFor(
      [this](std::shared_ptr<IAttributeModel> attr_model) -> bool {
        return showDialog(attr_model, AttrCreateViewArgs::getArgs(true));
      },
      shared_from_this());
}

void WorkAttributePresenter::reLayoutUI() {
  view()->beginReLayoutAttributes();

  showAttributes();

  if (model()->isEditMode()) {
    showCommands();
  }

  view()->endReLayoutAttributes();
}

void WorkAttributePresenter::showAttributes() {
  auto attr_models = model()->getAttributeModels();
  bool edit_mode = model()->isEditMode();

  for (auto& attr_model : attr_models) {
    auto attr_view = createRawViewIfNotExist<IAttributeView>(
        attr_model, AttrCreateViewArgs::getArgs(edit_mode));

    if (!attr_view)
      continue;

    auto location = model()->getLocation(attr_model.get());
    view()->addLabel(attr_model->displayName(),
                     location.row(), location.column() - 1,
                     location.row_span(), location.column_span());
    view()->addAttribute(attr_view,
                         location.row(), location.column(),
                         location.row_span(), location.column_span());
  }
}

void WorkAttributePresenter::showCommands() {
  auto erase_commands = model()->getEraseCommands();

  for (auto& command : erase_commands) {
    auto location = model()->getLocation(command);
    view()->addEraseCommand(command,
                      location.row(), location.column(),
                      location.row_span(), location.column_span());
  }

  auto popup_editor_commands = model()->getPopupEditorCommands();
  for (auto& command : popup_editor_commands) {
    auto location = model()->getLocation(command);
    view()->addPopupEditorCommand(command,
                                  location.row(), location.column(),
                                  location.row_span(), location.column_span());
  }

  auto add_attr_commands = model()->getAddAttributeCommands();
  for (auto & command : add_attr_commands) {
    auto location = model()->getLocation(command);
    view()->addAddAttributeCommand(command,
                                   location.row(), location.column(),
                                   location.row_span(), location.column_span());
  }
}
