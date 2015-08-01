// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QAction>
#include <QTimer>
#include <QDebug>

#include "src/qtui/attribute_layout.h"
#include "utils/command.h"
#include "qtui/i_attribute_view.h"
#include "snail/attribute_display_block.h"
#include "utils/u8string.h"

using namespace snailcore;  // NOLINT
using namespace utils;  // NOLINT

void AttributeLayout::beginLayout(int total_block_count) {
  // clear old status
  added_rows = 0;
  last_attr_type = ATTR_TYPE_NONE;
  last_label.clear();
  to_be_deleted_widgets = std::move(curr_widgets);
  to_be_removed_widgets = std::move(curr_attr_widgets);
  curr_widgets.clear();
  curr_attr_widgets.clear();

  total_item_count = total_block_count;
  left_side_count = total_block_count / 2;
  if (total_block_count % 2)
    ++left_side_count;
}

namespace {

bool should_move_group_to_right(int total_item_count, int sub_item_count) {
  if ( (total_item_count % 2) && (sub_item_count != 0) ) {
    // in this case, left side in therory plus one,
    // if we move the subattr to left, then left side
    // will plus three, to avoid this, we move the group
    // to the right side, which makes right_side_count = left_side_count + 1
    return true;
  }

  return (sub_item_count > 1);
}

bool should_move_subattr_to_left(int total_item_count, int sub_item_count)  {
  if ( (total_item_count % 2 == 0) && (sub_item_count == 1) ) {
    return true;
  } else {
    return false;
  }
}

}  // namespace

void AttributeLayout::mayAdjustLeftSideCountOnGroup(int sub_item_count) {
#if 1
  qDebug() << "left_side_count before adjust: " << left_side_count
           << ", total: " << total_item_count
           << ", sub: " << sub_item_count
           << ", added_rows: " << added_rows;
#endif

  if ((added_rows < left_side_count) &&
      (added_rows + 2 > left_side_count)) {
    // will split on first sub attr, need adjust left_side_count
    if (should_move_group_to_right(total_item_count, sub_item_count)) {
      --left_side_count;
    }

    if (should_move_subattr_to_left(total_item_count, sub_item_count)) {
      ++left_side_count;
    }
  }

  int num_group_attrs = sub_item_count + 1 /* group */;
  if (num_group_attrs == total_item_count)  {  // one and only one group
    if (sub_item_count <= 2)
      left_side_count = num_group_attrs;
  }

  qDebug() << "left_side_count after adjust: " << left_side_count;
}

utils::U8String AttributeLayout::label_to_display(utils::U8String label,
                                                  AttrType attr_type) {
  if ((attr_type == ATTR_TYPE_GROUP) ||
      (attr_type == ATTR_TYPE_TOPLEVEL_ATTR)) {
    last_attr_type = attr_type;
    last_label.clear();
    return label;
  } else {  // a sub attribute is to display
    if (last_attr_type == attr_type) {  // previous attr is also a sub attr
      if (label == last_label) {
        return utils::U8String {""};  // hide duplicate label
      } else {
        last_label = label;
        return label;
      }
    } else {  // previous attr is not a subattr
      last_attr_type = attr_type;
      last_label = label;
      return label;
    }
  }
}

void AttributeLayout::addPushButton(utils::Command* command,
                                    int row, int column) {
  QPushButton* button = new QPushButton();
  button->setObjectName(QStringLiteral("addAttrButton"));
  QString tooltip = U8StringToQString(command->display_text());
  button->setToolTip(tooltip);
  connect(button, &QPushButton::clicked,
          [command]() {
            command->redo();
          });
  addWidget(button, row, column);
  curr_widgets.push_front(button);
}

void* AttributeLayout::layoutAttributeGroupDisplayBlock(
    AttributeGroupDisplayBlock attr_group_block) {
  mayAdjustLeftSideCountOnGroup(attr_group_block.sub_attr_count);

  int row = added_rows;
  int label_column = kLeftLabelColumn;
  int add_btn_column = kLeftAddCommandColumn;

  ++added_rows;

  if (row >= left_side_count) {
    row -= left_side_count;
    label_column = kRightLabelColumn;
    add_btn_column = kRightAddCommandColumn;
  }

  utils::U8String label_ustr = label_to_display(attr_group_block.label,
                                                ATTR_TYPE_GROUP);

  QString label_qstr = U8StringToQString(label_ustr);

  auto label = new QLabel(label_qstr);
  addWidget(label, row, label_column);
  curr_widgets.push_front(label);

  qDebug() << "add group " << label_qstr << " at row " << row;

  if (attr_group_block.add_command)
    addPushButton(attr_group_block.add_command, row, add_btn_column);

  return label;
}

#if 0
void AttributeLayout::addToolButton(utils::Command* command,
                                    int row, int column) {
  QToolButton* button = new QToolButton();

  QAction* action = new QAction(button);
  action->setObjectName(QStringLiteral("eraseOrEditAttrAction"));
  QString action_tooltip = U8StringToQString(command->display_text());
  action->setStatusTip(action_tooltip);
  action->setEnabled(true);
  connect(action, &QAction::triggered,
          [command]() {
            command->redo();
          });
  button->setDefaultAction(action);

  addWidget(button, row, column);
  curr_widgets.push_front(button);
}
#endif

void* AttributeLayout::layoutAttributeDisplayBlock(
    AttributeViewDisplayBlock attr_view_block) {
  int row = added_rows;
  int label_column = kLeftLabelColumn;
  int attr_view_column = kLeftAttrViewColumn;

  ++added_rows;

  if (row >= left_side_count) {
    row -= left_side_count;
    label_column = kRightLabelColumn;
    attr_view_column = kRightAttrViewColumn;
  }

#if 1
  qDebug() << "add attr " << U8StringToQString(attr_view_block.label)
           << " at row " << row;
#endif

  AttrType attr_type = ATTR_TYPE_TOPLEVEL_ATTR;
  if (attr_view_block.is_in_group)
    attr_type = ATTR_TYPE_SUB_ATTR;

  utils::U8String label_ustr =
      label_to_display(attr_view_block.label, attr_type);

  QString label_qstr = U8StringToQString(label_ustr);
  auto label = new QLabel(label_qstr);
  addWidget(label, row, label_column);
  curr_widgets.push_front(label);

  QWidget* attr_widget = attr_view_block.attr_view->getWidget();
  // NOTE: if cause flicker, use RelocatableGridLayout
  removeWidget(attr_widget);
  to_be_removed_widgets.remove_if(
      [attr_widget](const QWidget* widget) {
        return widget == attr_widget;
      });
  addWidget(attr_widget, row, attr_view_column);
  curr_attr_widgets.push_front(attr_widget);

  return label;
}

void AttributeLayout::updateLabel(UpdateAttrLabelData label_data) {
  QLabel* label = reinterpret_cast<QLabel*>(label_data.view_priv_data);
  label->setText(U8StringToQString(label_data.label));
}

void AttributeLayout::endLayout() {
  QTimer::singleShot(0, this, SLOT(clearOldWidgets()));
}

void AttributeLayout::clearOldWidgets() {
  for (auto widget : to_be_deleted_widgets) {
    removeWidget(widget);
    widget->deleteLater();
  }

  to_be_deleted_widgets.clear();

  for (auto widget : to_be_removed_widgets) {
    removeWidget(widget);
    // TODO(lutts): how to remove triad?
  }
  to_be_removed_widgets.clear();
}
