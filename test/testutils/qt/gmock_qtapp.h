// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_QT_GMOCK_QTAPP_H_
#define TEST_TESTUTILS_QT_GMOCK_QTAPP_H_

#include <QApplication>

class QAppEnvironment : public testing::Environment {
 public:
  virtual void SetUp() {
    printf("%s", "QAppEnvironment::SetUp() called.\n");
    qt_app_ = new QApplication(dummy_argc, nullptr);
  }

  virtual void TearDown() {
    printf("%s", "QAppEnvironment::TearDown() called.\n");
    delete qt_app_;
  }

  QApplication* qt_app_;
  int dummy_argc { 0 };
};

GTEST_API_ int main(int argc, char** argv) {
  // Since Google Mock depends on Google Test, InitGoogleMock() is
  // also responsible for initializing Google Test.  Therefore there's
  // no need for calling testing::InitGoogleTest() separately.
  testing::InitGoogleMock(&argc, argv);

  testing::AddGlobalTestEnvironment(new QAppEnvironment);

  return RUN_ALL_TESTS();
}

#endif  // TEST_TESTUTILS_QT_GMOCK_QTAPP_H_
