//-*- TestCaseName: QModelIndexGeneratorTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "test/testutils/qmodelindex_generator.h"

class QModelIndexGeneratorTest : public ::testing::Test {
 protected:
  QModelIndexGeneratorTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~QModelIndexGeneratorTest() { }
  virtual void SetUp() {}
  // virtual void TearDown() { }

  // region: test subject
  xtestutils::QModelIndexGenerator generator;
  // endregion
};

TEST_F(QModelIndexGeneratorTest,
       should_generate_QModelIndex_with_specified_row_and_column) {  // NOLINT
  // Setup fixture
  int expect_row = std::rand();
  int expect_column = std::rand();

  // Exercise system
  auto index = generator.index(expect_row, expect_column);

  // Verify results
  ASSERT_TRUE(index.isValid());
  ASSERT_EQ(expect_row, index.row());
  ASSERT_EQ(expect_column, index.column());
}
