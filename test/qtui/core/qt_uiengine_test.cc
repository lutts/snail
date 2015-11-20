//-*- TestCaseName: QtUiEngineTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <QAction>
#include <QTimer>
#include <QDebug>

#include "test/testutils/gmock_common.h"

#include "src/qtui/core/qt_uiengine.h"

class QtUiEngineTest : public QObject, public ::testing::Test {
  Q_OBJECT

 protected:
  QtUiEngineTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~QtUiEngineTest() { }
  virtual void SetUp() { uiEngine = utils::make_unique<QtUiEngine>(); }
  virtual void TearDown() { uiEngine->quit(); }

 public slots:
  virtual void quitApp() { uiEngine->quit(); }

 public:
  std::unique_ptr<QtUiEngine> uiEngine;
};

#include "qt_uiengine_test.moc"

class MockListener {
 public:
  explicit MockListener(snailcore::IUiEngine* uiEngine) {
    uiEngine->whenAboutToQuit([this]() { AboutToQuit(); }, nullptr);
  }

  MOCK_METHOD0(AboutToQuit, void());
};

// QApplication can not new/delete twice,
// see https://bugreports.qt.io/browse/QTBUG-41145
// fixes: https://codereview.qt-project.org/#/c/97357/
//
// because of this bug, we put all tests in one TestCase
//
TEST_F(QtUiEngineTest,
       should_work_as_expected_large_testcase_because_qt_bug) {  // NOLINT
  // Test1: can create widgets
  {
    QAction* action = new QAction(this);
    delete action;
  }

  // Test2:
  {
    MockListener mockListener(uiEngine.get());
    EXPECT_CALL(mockListener, AboutToQuit());

    QTimer::singleShot(300, this, SLOT(quitApp()));
    uiEngine->run();
  }
}
