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

#include "src/qtui/ui/widgets/qtlineedit.h"
#include "src/qtui/ui/widgets/qtcompleter.h"
#include "qtui/i_kbnode_tree_qmodel.h"

KbNodeAttributeEditView::KbNodeAttributeEditView() {
  QVBoxLayout* vbox_layout = new QVBoxLayout();
  vbox_layout->setMargin(0);
  vbox_layout->setSpacing(0);

  line_edit_ = new QtLineEdit(this);
  auto completer = new QtCompleter(line_edit_);
  auto drop_down_tree_view = new QTreeView(line_edit_);
  drop_down_tree_view->header()->hide();
  completer->setPopup(drop_down_tree_view);
  line_edit_->setQtCompleter(completer);

  QObject::connect(completer, &QtCompleter::clicked,
                   [this](const QModelIndex& index) {
                     UserClickedIndex(index);
                   });

  QObject::connect(completer, &QtCompleter::filterPatternChanged,
                   [this](const QString& filter_pattern) {
                     FilterPatternChanged(filter_pattern);
                   });

  QObject::connect(line_edit_, &QLineEdit::editingFinished,
                   [this]() {
                     EditingFinished(line_edit_->text());
                   });

  vbox_layout->addWidget(line_edit_);

  err_msg_label_ = new QLabel(this);
  err_msg_label_->hide();

  connect(err_msg_label_, &QLabel::linkActivated,
          [this](const QString &link) {
            (void)link;
            UserClickAddKbNode();
          });

  vbox_layout->addWidget(err_msg_label_);

  setLayout(vbox_layout);
}

KbNodeAttributeEditView::~KbNodeAttributeEditView() = default;

void KbNodeAttributeEditView::setKbNodeName(const QString& kbnode_name) {
  line_edit_->setText(kbnode_name);
}

void KbNodeAttributeEditView::setKbNodeTreeQModel(IKbNodeTreeQModel* kbnode_tree_qmodel) {
  auto model = dynamic_cast<QAbstractItemModel*>(kbnode_tree_qmodel);
  auto completer = line_edit_->qtcompleter();
  completer->setModel(model);
}

void KbNodeAttributeEditView::warnMultipleMatch() {
  // TODO(lutts): write a good description
  err_msg_label_->setText(tr("Multiple XXX matched the entered value"));
  err_msg_label_->show();
}

void KbNodeAttributeEditView::warnNotFound() {
  err_msg_label_->setText(tr("<html><head/><body><p><a href=\"#add_kbnode\"><span style=\" text-decoration: underline; color:#0000ff;\">Add XXX</span></a></p></body></html>")); // NOLINT
  err_msg_label_->show();
}
