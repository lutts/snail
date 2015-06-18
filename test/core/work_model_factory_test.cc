// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/utils.h"
#include "test/testutils/generic_mock_listener.h"
#include "test/testutils/slot_catcher.h"
#include "test/testutils/mock_object_generator.h"
#include "test/testutils/model_view_mock_generator.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

class WorkModelFactoryTest : public ::testing::Test {
  //-*- TestCaseName: WorkModelFactoryTest;-*-
 protected:
  WorkModelFactoryTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkModelFactoryTest() { }
  virtual void SetUp() {
    // Setup fixture

    // Expectations which only need run once

    // Expectations which need run every test case

    // Excercise system
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  // endregion

  // region: test subject

  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(WorkModelFactoryTest, should_construct_properly) { // NOLINT
  // See SetUp()
  SUCCEED();
}
