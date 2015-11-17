// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/ui/kbnode_attribute_edit_view.h"

#include <QVBoxLayout>
#include <QTreeView>
#include <QHeaderView>
#include <QLabel>

#include "src/qtui/ui/widgets/filterwidget.h"
#include "src/qtui/ui/widgets/qtcompleter.h"
#include "qtui/core/fto_tree_item_qmodel.h"

KbNodeAttributeEditView::KbNodeAttributeEditView() {
  QVBoxLayout* vbox_layout = new QVBoxLayout();
  vbox_layout->setMargin(0);
  vbox_layout->setSpacing(0);

  filter_widget_ = new FilterWidget(this);
  auto completer = new QtCompleter(filter_widget_);
  auto drop_down_tree_view = new QTreeView(filter_widget_);
  drop_down_tree_view->header()->hide();
  completer->setPopup(drop_down_tree_view);
  filter_widget_->setQtCompleter(completer);

  QObject::connect(
      completer, &QtCompleter::clicked,
      [this](const QModelIndex& index) { UserClickedIndex(index); });

  QObject::connect(completer, &QtCompleter::filterPatternChanged,
                   [this](const QString& filter_pattern) {
    FilterPatternChanged(filter_pattern);
  });

  QObject::connect(filter_widget_, &QLineEdit::editingFinished,
                   [this]() { EditingFinished(filter_widget_->text()); });

  vbox_layout->addWidget(filter_widget_);

  err_msg_label_ = new QLabel(this);
  err_msg_label_->hide();

  connect(err_msg_label_, &QLabel::linkActivated, [this](const QString& link) {
    (void)link;
    UserClickAddKbNode();
  });

  vbox_layout->addWidget(err_msg_label_);

  setLayout(vbox_layout);
}

KbNodeAttributeEditView::~KbNodeAttributeEditView() = default;

void KbNodeAttributeEditView::setKbNodeName(const QString& kbnode_name) {
  filter_widget_->setText(kbnode_name);
}

void KbNodeAttributeEditView::setKbNodeTreeQModel(
    QAbstractItemModel* kbnode_tree_qmodel) {
  auto completer = filter_widget_->qtcompleter();
  completer->setModel(kbnode_tree_qmodel);
}

void KbNodeAttributeEditView::warnMultipleMatch(const QString& provider_name) {
  (void)provider_name;
  err_msg_label_->setText(
      tr("Entered value matched multiple candidate, \nplease select one from "
         "the drop down list."));  // NOLINT;
  err_msg_label_->show();
}

void KbNodeAttributeEditView::warnNotFound(const QString& provider_name) {
  err_msg_label_->setText(
      tr("<html><head/><body><p>Entered value not found, you can <a "
         "href=\"#add_kbnode\"><span style=\" text-decoration: underline; "
         "color:#0000ff;\">add new %1 value</span></a></p></body></html>")
          .arg(provider_name));  // NOLINT
  err_msg_label_->show();
}

void KbNodeAttributeEditView::clearWarningMessages() { err_msg_label_->hide(); }
