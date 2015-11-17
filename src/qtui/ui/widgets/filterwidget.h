// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_UI_WIDGETS_FILTERWIDGET_H_
#define SRC_QTUI_UI_WIDGETS_FILTERWIDGET_H_

#include <QLineEdit>
#include <QTimer>
#include <QEvent>

class QtCompleter;

class FilterWidget : public QLineEdit {
  Q_OBJECT

 public:
  explicit FilterWidget(QWidget* parent = nullptr);
  virtual ~FilterWidget() = default;

  void setQtCompleter(QtCompleter* completer);
  QtCompleter* qtcompleter();

 private slots:
  void on_textEdited(const QString& text);
  void slot_show_completer_popup();
  void complete();

 private:
  enum { kShowCompletionEvent = QEvent::User };

  FilterWidget(const FilterWidget& other) = delete;
  FilterWidget& operator=(const FilterWidget& other) = delete;

  void focusInEvent(QFocusEvent* e);
  void focusOutEvent(QFocusEvent* e);
  void mouseReleaseEvent(QMouseEvent* e) override;
  void customEvent(QEvent* e);
  void mayShowCompleterOnEmptyContent();

  void complete(const QString& filter_pattern);

  QtCompleter* completer_{nullptr};
  QTimer timer;
};

#endif  // SRC_QTUI_UI_WIDGETS_FILTERWIDGET_H_
