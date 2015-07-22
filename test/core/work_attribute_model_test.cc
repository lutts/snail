//-*- TestCaseName: WorkAttributeModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/work_attribute_model.h"
#include "core/mock_attribute_container.h"
#include "core/mock_attribute.h"
#include "core/mock_attribute_supplier.h"
#include "snail/mock_attribute_model_factory.h"
#include "utils/mock_relay_command_factory.h"

using namespace utils;  // NOLINT
using namespace utils::tests;  // NOLINT

namespace snailcore {
namespace tests {

class WorkAttributeModelTest : public ::testing::Test {
 protected:
  WorkAttributeModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkAttributeModelTest() { }
  virtual void SetUp() {
    setupAttributeContainer();

    RECORD_USED_MOCK_OBJECTS_SETUP;

    R_EXPECT_CALL(attr_container, getAttributes())
        .WillOnce(Return(attributes));
    for (auto & attr : attributes) {
      R_EXPECT_CALL(attr_model_factory, createModel())
          .WillOnce(Return(attr_model));
    }

    model = utils::make_unique<WorkAttributeModel>(
        &attr_container,
        attr_model_factory,
        relay_cmd_factory);

    VERIFY_RECORDED_MOCK_OBJECTS;

    // TODO(lutts): check layout results
  }
  // virtual void TearDown() { }

  void setupAttributeContainer();

  // region: objects test subject depends on
  MockAttributeContainer attr_container;
  MockAttributeModelFactory attr_model_factory;
  MockRelayCommandFactory relay_cmd_factory;
  // endregion

  // region: test subject
  std::unique_ptr<IWorkAttributeModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

void WorkAttributeModelTest::setupAttributeContainer() {
}

TEST_F(WorkAttributeModelTest, should_construct_properly) { // NOLINT
  // See SetUp()
  SUCCEED();
}

}  // namespace tests
}  // namespace snailcore
