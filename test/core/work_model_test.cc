//-*- TestCaseName: WorkModelTest;-*-
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

#include "src/core/work_model.h"
#include "snail/mock_work.h"

namespace snailcore {
namespace tests {

class WorkModelTest : public ::testing::Test {
 protected:
  WorkModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkModelTest() { }
  virtual void SetUp() {
    // Setup fixture

    // Expectations which only need run once

    // Expectations which need run every test case

    // Excercise system
    work_model = utils::make_unique<WorkModel>(&work);
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  MockWork work;
  // endregion

  // region: test subject
  std::unique_ptr<IWorkModel> work_model;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(WorkModelTest, should_be_able_to_get_back_the_backing_work) { // NOLINT
  ASSERT_EQ(&work, work_model->getWork());
}

TEST_F(WorkModelTest, should_name_be_the_backing_work_name) { // NOLINT
  // Setup fixture
  auto work_name = xtestutils::genRandomString();

  ON_CALL(work, name()).WillByDefault(ReturnRef(work_name));

  // Verify results
  ASSERT_EQ(work_name, work_model->name());
}

}  // namespace tests
}  // namespace snailcore
