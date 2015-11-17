// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/ui/attribute_set_view.h"

#include "qsint/include/QSint"

AttributeSetViewForWorkView::AttributeSetViewForWorkView()
    : action_group_(new QSint::ActionGroup(tr("Attributes"))),
      attr_set_container_(new QWidget(action_group_)) {
  action_group_->setHasEditButton(true);
  action_group_->addWidget(attr_set_container_);

  QObject::connect(action_group_, &QSint::ActionGroup::editButtonClicked,
                   [this]() { UserSwitchMode(); });
}

QWidget* AttributeSetViewForWorkView::getWidget() { return action_group_; }

void AttributeSetViewForWorkView::switchToDisplayMode() {
  action_group_->setEditButtonText(QStringLiteral("[Edit]"));
}

void AttributeSetViewForWorkView::switchToEditMode() {
  action_group_->setEditButtonText(QStringLiteral("[Done]"));
}

void AttributeSetViewForWorkView::setSwitchModelButtonEnabled(bool enabled) {
  action_group_->setEditButtonEnabled(enabled);
}
