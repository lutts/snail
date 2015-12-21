// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QApplication>
#include <QFocusEvent>
#include <QIcon>
#include <QToolButton>
#include <QWidgetAction>
#include <QDebug>

#include "src/qtui/ui/widgets/filterwidget.h"
#include "src/qtui/ui/widgets/qtcompleter.h"

FilterWidget::FilterWidget(QWidget* parent) : QLineEdit(parent) {
  setClearButtonEnabled(true);
  connect(this, SIGNAL(textEdited(const QString&)), this,
          SLOT(on_textEdited(const QString&)));

  const QIcon icon = QIcon(QPixmap(QStringLiteral(":/images/find.png")));
  QToolButton* optionsButton = new QToolButton;
#ifndef QT_NO_CURSOR
  optionsButton->setCursor(Qt::ArrowCursor);
#endif
  optionsButton->setFocusPolicy(Qt::NoFocus);
  optionsButton->setStyleSheet(QStringLiteral("* { border: none; }"));
  optionsButton->setIcon(icon);

  QWidgetAction* optionsAction = new QWidgetAction(this);
  optionsAction->setDefaultWidget(optionsButton);
  addAction(optionsAction, QLineEdit::LeadingPosition);

  connect(optionsButton, SIGNAL(clicked()), this, SLOT(complete()));
}

void FilterWidget::setQtCompleter(QtCompleter* completer) {
  if (completer == completer_) return;

  if (completer_) {
    completer_->setWidget(nullptr);
    if (completer_->parent() == this) delete completer_;
  }

  completer_ = completer;

  if (!completer_) return;

  if (completer_->widget() == nullptr) completer_->setWidget(this);
}

QtCompleter* FilterWidget::qtcompleter() { return completer_; }

void FilterWidget::focusInEvent(QFocusEvent* e) {
  QLineEdit::focusInEvent(e);

  if (completer_ && ((e->reason() == Qt::MouseFocusReason) ||
                     (e->reason() == Qt::TabFocusReason))) {
    completer_->setWidget(this);
    complete();
  }
}

void FilterWidget::focusOutEvent(QFocusEvent* e) {
  QLineEdit::focusOutEvent(e);
}

void FilterWidget::mouseReleaseEvent(QMouseEvent* e) {
  QLineEdit::mouseReleaseEvent(e);
  mayShowCompleterOnEmptyContent();
}

void FilterWidget::customEvent(QEvent* e) {
  QLineEdit::customEvent(e);
  if (e->type() == static_cast<QEvent::Type>(kShowCompletionEvent)) {
    // force completion after text is deleted
    complete();
  }
}

void FilterWidget::mayShowCompleterOnEmptyContent() {
  if (text().isEmpty()) {
    timer.singleShot(0, this, SLOT(slot_show_completer_popup()));
  }
}

void FilterWidget::slot_show_completer_popup() {
  // apparently, complete() works only in event handler
  QApplication::postEvent(
      this, new QEvent(static_cast<QEvent::Type>(kShowCompletionEvent)));
}

void FilterWidget::clearFilterText() { filter_text_.clear(); }

void FilterWidget::on_textEdited(const QString& text) {
  filter_text_ = text;
  complete();
}

void FilterWidget::complete() {
  if (!completer_ || isReadOnly() || echoMode() != QLineEdit::Normal) return;

  completer_->setFilterPattern(filter_text_);
  completer_->complete();
}
