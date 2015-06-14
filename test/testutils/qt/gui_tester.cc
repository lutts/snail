// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <QWidget>
#include <QTimer>
#include <QtTest/QtTest>

#include "test/testutils/qt/gui_tester.h"

void DelayedAction::run() {
  if (next)
    QTimer::singleShot(next->delay, next, SLOT(run()));
}

/*
  Schedules a mouse click at a widget using a singleShot timer.
  Only one click can be scheduled at a time.
*/
class ClickLaterAction : public DelayedAction {
  Q_OBJECT
 public:
  explicit ClickLaterAction(QWidget *widget,
                            Qt::MouseButton button = Qt::LeftButton) {
    this->widget  = widget;
    this->button = button;
  }

 protected slots:
  void run() override {
    QTest::mouseClick(widget, button);

    DelayedAction::run();
  }

 private:
  QWidget *widget;
  Qt::MouseButton button;
};

class DoubleClickLaterAction : public DelayedAction {
  Q_OBJECT

 public:
  explicit DoubleClickLaterAction(QWidget *widget,
                                  Qt::MouseButton button = Qt::LeftButton) {
    this->widget  = widget;
    this->button = button;
  }

 protected slots:
  void run() override {
    QTest::mouseDClick(widget, button);

    DelayedAction::run();
  }

 private:
  QWidget *widget;
  Qt::MouseButton button;
};

class KeyClickLaterAction : public DelayedAction {
  Q_OBJECT

 public:
  explicit KeyClickLaterAction(QWidget *widget, Qt::Key key,
                               Qt::KeyboardModifiers modifier = Qt::NoModifier) { // NOLINT
    this->widget = widget;
    this->key = key;
    this->modifier = modifier;
  }

 protected slots:
  void run() override {
    QTest::keyClick(widget, key);

    DelayedAction::run();
  }

 private:
  QWidget *widget;
  Qt::Key key;
  Qt::KeyboardModifiers modifier;
};

////////////////////////////////////////////////////////////

GuiTester::GuiTester() {
  clearSequence();
}

GuiTester::~GuiTester() {
  foreach(DelayedAction *action, actions)
      delete action;
}

void GuiTester::clickLater(QWidget *widget,
                           int delay_msec,
                           Qt::MouseButton button) {
  createSingleActionSeq(new ClickLaterAction(widget, button), delay_msec);
}

void GuiTester::doubleClickLater(QWidget *widget,
                                 int delay_msec,
                                 Qt::MouseButton button) {
  createSingleActionSeq(new DoubleClickLaterAction(widget, button), delay_msec);
}

void GuiTester::keyClickLater(QWidget *widget,
                              Qt::Key key,
                              Qt::KeyboardModifiers modifier,
                              int delay_msec) {
  createSingleActionSeq(new KeyClickLaterAction(widget, key, modifier),
                        delay_msec);
}

void GuiTester::clearSequence() {
  startAction = new DelayedAction();
  actions.insert(startAction);
  lastAction = startAction;
}

void GuiTester::addToSequence(DelayedAction *action, int delay) {
  actions.insert(action);
  action->delay = delay;
  lastAction->next = action;
  lastAction = action;
}

void GuiTester::runSequence() {
  QTimer::singleShot(0, startAction, SLOT(run()));
}

void GuiTester::createSingleActionSeq(DelayedAction* action, int delay_msec) {
  clearSequence();
  addToSequence(action, delay_msec);
  runSequence();
}



#include "moc_gui_tester_cc.cc"
