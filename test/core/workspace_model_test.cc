//-*- TestCaseName: WorkSpaceModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/workspace_model.h"
#include "core/factory/mock_work_model_factory.h"
#include "core/factory/mock_work_factory.h"
#include "snail/mock_work_model.h"

namespace snailcore {
namespace tests {

class WorkSpaceModelTest : public ::testing::Test {
 protected:
  WorkSpaceModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkSpaceModelTest() { }
  virtual void SetUp() {
    workspace_model =
        utils::make_unique<WorkSpaceModel>(&work_model_factory, work_factory);
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  MockWorkFactory work_factory;
  MockWorkModelFactory work_model_factory;
  // endregion

  // region: test subject
  std::unique_ptr<IWorkSpaceModel> workspace_model;
  // endregion

  // region: object depends on test subject
  // endregion
};

class MockListener {
 public:
  explicit MockListener(IWorkSpaceModel* workspace_model) {
    workspace_model->whenWorkModelAdded(
        [this](std::shared_ptr<IWorkModel> work_model) {
          WorkModelAdded(work_model);
        },
        nullptr);

    workspace_model->whenActiveWorkModelChanged([this](IWorkModel* work_model) {
      ActiveWorkModelChanged(work_model);
    }, nullptr);

    workspace_model->whenWorkModelActivelyRemoved(
        [this](IWorkModel* work_model) {
          WorkModelActivelyRemoved(work_model);
        },
        nullptr);
  }

  MOCK_METHOD1(WorkModelAdded, void(std::shared_ptr<IWorkModel> work_model));
  MOCK_METHOD1(ActiveWorkModelChanged, void(IWorkModel* work_model));
  MOCK_METHOD1(WorkModelActivelyRemoved, void(IWorkModel* work_model));
};

TEST_F(WorkSpaceModelTest, should_be_able_to_create_work) {  // NOLINT
  // Setup fixture
  auto work = xtestutils::genDummyPointer<fto::Work>();
  auto work_model = std::make_shared<MockWorkModel>();
  auto work_name = xtestutils::genRandomString();

  // Expectations
  EXPECT_CALL(work_factory, create(work_name)).WillOnce(Return(work));
  EXPECT_CALL(work_model_factory, createWorkModel())
      .WillOnce(Return(work_model));
  EXPECT_CALL(*work_model, set_work(work));

  // TODO(lutts): NameChanged

  MockListener mockListener(workspace_model.get());
  std::shared_ptr<IWorkModel> work_imodel = work_model;

  EXPECT_CALL(mockListener, WorkModelAdded(work_imodel));

  // Exercise system
  workspace_model->createWork(work_name);
}

}  // namespace tests
}  // namespace snailcore
