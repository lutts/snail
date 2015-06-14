// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_QT_GUI_TESTER_H_
#define TEST_TESTUTILS_QT_GUI_TESTER_H_

#include <QSet>
#include <QWindow>
#include <QWidget>
#include <QtTest/QtTest>
#include <QElapsedTimer>
#include <QCoreApplication>

class DelayedAction : public QObject {
  Q_OBJECT
 public:
  DelayedAction() : delay(0), next(nullptr) { }
  virtual ~DelayedAction() { }

 public slots:
  virtual void run();

 public:
  int delay;
  DelayedAction *next;
};

class GuiTester : public QObject {
  Q_OBJECT

 public:
  GuiTester();
  virtual ~GuiTester();

 protected:
  void clickLater(QWidget *widget,
                  int delay_msec = 0,
                  Qt::MouseButton button = Qt::LeftButton);

  void doubleClickLater(QWidget *widget,
                        int delay_msec = 0,
                        Qt::MouseButton button = Qt::LeftButton);

  void keyClickLater(QWidget *widget,
                     Qt::Key key,
                     Qt::KeyboardModifiers modifier = Qt::NoModifier,
                     int delay_msec = 0);

  void clearSequence();
  void addToSequence(DelayedAction *action, int delay = 0);
  void runSequence();

  inline static QWindow* qWaitForNamedWindowExposed(
      QObject *parent,
      QString objectName,
      Qt::FindChildOptions options = Qt::FindChildrenRecursively,
      int timeout = 1000,
      int complain_threshold = 30) {
    QElapsedTimer timer;
    timer.start();

    QWindow *window = nullptr;

    while (!window || !window->isExposed()) {
      int remaining = timeout - static_cast<int>(timer.elapsed());
      if (remaining <= 0)
        break;

      if (timer.elapsed() > complain_threshold) {
        qDebug() << "WARNING: Window " << objectName
                 << " still not exposed after "
                 << complain_threshold << "ms";
      }

      QCoreApplication::processEvents(QEventLoop::AllEvents, remaining);
      QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);
      QTest::qSleep(1);

      window = parent->findChild<QWindow*>(objectName, options);
    }

    return window;
  }

  inline static QWidget* qWaitForNamedWidgetVisible(
      QObject *parent,
      QString objectName,
      Qt::FindChildOptions options = Qt::FindChildrenRecursively,
      int timeout = 1000,
      int complain_threshold = 30) {
    QElapsedTimer timer;
    timer.start();

    QWidget *widget = nullptr;

    while (!widget || !widget->isVisible()) {
      int remaining = timeout - static_cast<int>(timer.elapsed());
      if (remaining <= 0)
        break;

      if (timer.elapsed() > complain_threshold) {
        qDebug() << "WARNING: Widget " << objectName
                 << " still not visible after "
                 << timer.elapsed() << "ms";
      }

      QCoreApplication::processEvents(QEventLoop::AllEvents, remaining);
      QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);
      QTest::qSleep(1);

      widget = parent->findChild<QWidget*>(objectName, options);
    }

    return widget;
  }


  inline static bool qWaitNamedWidgetDeleted(
      QObject *parent,
      QString objectName,
      Qt::FindChildOptions options = Qt::FindChildrenRecursively,
      int timeout = 1000,
      int complain_threshold = 30) {
    QElapsedTimer timer;
    timer.start();

    QWidget *widget = parent->findChild<QWidget*>(objectName, options);

    while (widget) {
      int remaining = timeout - static_cast<int>(timer.elapsed());
      if (remaining <= 0) {
        return false;
      }

      if (timer.elapsed() > complain_threshold) {
        qDebug() << "WARNING: Widget " << objectName
                 << " still exist after "
                 << timer.elapsed() << "ms";
      }

      QCoreApplication::processEvents(QEventLoop::AllEvents, remaining);
      QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);
      QTest::qSleep(1);

      widget = parent->findChild<QWidget*>(objectName, options);
    }

    return true;
  }

  inline static bool qWaitTopLevelWidgetDismiss(QString objectName,
                                                int timeout = 1000,
                                                int complain_threshold = 30) {
    bool dismissed = false;

    QElapsedTimer timer;
    timer.start();

    for (;;) {
      bool still_visible = false;
      // TODO(lutts): use QApplication::activeModalWidget()
      QWidgetList topWidgets = QApplication::topLevelWidgets();
      for (QWidget *w : topWidgets) {
        if (objectName == w->objectName()) {
          still_visible = true;
          break;
        }
      }

      if (!still_visible) {
        dismissed = true;
        break;
      }

      int remaining = timeout - static_cast<int>(timer.elapsed());
      if (remaining <= 0)
        break;

      if (timer.elapsed() > complain_threshold) {
        qDebug() << "WARNING: Top level widget " << objectName
                 << " still visible after "
                 << complain_threshold << "ms";
      }

      QCoreApplication::processEvents(QEventLoop::AllEvents, remaining);
      QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);
      QTest::qSleep(1);
    }  // for(;;)

    return dismissed;
  }

 private:
  void createSingleActionSeq(DelayedAction* action, int delay_msec);

  QSet<DelayedAction *> actions;
  DelayedAction *startAction;
  DelayedAction *lastAction;
};



#endif  // TEST_TESTUTILS_QT_GUI_TESTER_H_
