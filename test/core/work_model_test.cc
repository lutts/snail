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

#include "snail/mock_attribute_set_model.h"
#include "core/mock_attribute_set_model_factory.h"

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
    EXPECT_CALL(work, whenNameChanged(_, _))
        .WillOnce(SaveArg<0>(&workNameChanged));

    auto model = std::make_shared<WorkModel>(&attr_set_model_factory);
    model->set_work(&work);
    work_model = std::move(model);
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  MockWork work;
  MockAttributeSetModelFactory attr_set_model_factory;
  // endregion

  // region: test subject
  std::shared_ptr<IWorkModel> work_model;
  // endregion

  // region: object depends on test subject
  SlotCatcher<IWork::NameChangedSlotType> workNameChanged;
  // endregion
};

class MockListener : public GenericMockListener<MockListener,
                                                IWorkModel> {
 public:
  MOCK_METHOD1(NameChanged, void(const utils::U8String& new_name));

  void bindListenerMethods(std::shared_ptr<utils::ITrackable> trackObject,
                           IWorkModel* model) {
    model->whenNameChanged(
        [this](const utils::U8String& new_name) {
          NameChanged(new_name);
        },
        trackObject);
  }
};

TEST_F(WorkModelTest, should_name_be_the_backing_work_name) { // NOLINT
  // Setup fixture
  auto work_name = xtestutils::genRandomString();

  ON_CALL(work, name()).WillByDefault(ReturnRef(work_name));

  // Verify results
  ASSERT_EQ(work_name, work_model->name());
}

TEST_F(WorkModelTest, should_relay_set_name_to_backing_work) { // NOLINT
  // Setup fixture
  auto tester = [this](bool expect_result) {
    auto new_name = xtestutils::genRandomString();

    // Expectations
    EXPECT_CALL(work, set_name(new_name))
    .WillOnce(Return(expect_result));

    // Verify results
    ASSERT_EQ(expect_result, work_model->set_name(new_name));
  };

  tester(true);
  tester(false);
}

TEST_F(WorkModelTest, should_relay_NameChanged_signal_fired_by_backing_work) { // NOLINT
  auto new_name = xtestutils::genRandomString();
  // Expectations
  auto mockListener = MockListener::attachTo(work_model.get());
  EXPECT_CALL(*mockListener, NameChanged(new_name));

  // Exercise system
  workNameChanged(new_name);
}

TEST_F(WorkModelTest,
       should_be_able_to_create_attribute_set_model) { // NOLINT
  // Setup fixture
  auto expect_attr_set_model =
      std::make_shared<MockAttributeSetModel>();

  std::vector<IAttributeSupplier*> attr_suppliers;
  attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
  attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
  attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());

  // Expectations
  EXPECT_CALL(work, attributeSuppliers())
      .WillOnce(Return(attr_suppliers));
  EXPECT_CALL(attr_set_model_factory,
              createAttributeSetModel(attr_suppliers))
      .WillOnce(Return(expect_attr_set_model));

  // Exercise system
  auto attr_set_model = work_model->createAttributeSetModel();

  // Verify results
  ASSERT_EQ(expect_attr_set_model, attr_set_model);
}

}  // namespace tests
}  // namespace snailcore
