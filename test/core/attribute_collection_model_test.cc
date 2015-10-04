//-*- TestCaseName: AttributeCollectionModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/attribute_collection_model.h"
#include "core/mock_attribute_model_factory.h"
#include "snail/mock_attribute_model.h"

namespace snailcore {
namespace tests {

class AttributeCollectionModelTest : public ::testing::Test {
 protected:
  AttributeCollectionModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeCollectionModelTest() { }
  virtual void SetUp() {
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());

    model = utils::make_unique<AttributeCollectionModel>(attr_suppliers,
                                                         attr_model_factory);
  }
  // virtual void TearDown() { }

  void checkCreateAttributeModel(
      std::shared_ptr<MockAttributeModel> attr_model,
      SlotCatcher<IAttributeModel::ValidateCompleteSlotType>* validateComplete);

  // region: objects test subject depends on
  std::vector<IAttributeSupplier*> attr_suppliers;
  MockAttributeModelFactory attr_model_factory;
  // endregion

  // region: test subject
  std::unique_ptr<AttributeCollectionModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

BEGIN_MOCK_LISTENER_DEF(MockListener, IAttributeCollectionModel)

MOCK_METHOD0(SwitchToEditMode, void());
MOCK_METHOD0(SwitchToDisplayMode, void());
MOCK_METHOD1(ValidateComplete, void(bool validate_result));

BEGIN_BIND_SIGNAL(IAttributeCollectionModel)

BIND_SIGNAL0(SwitchToEditMode, void);
BIND_SIGNAL0(SwitchToDisplayMode, void);
BIND_SIGNAL1(ValidateComplete, void, bool, validate_result);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()

TEST_F(AttributeCollectionModelTest,
       should_be_able_to_get_the_attribute_suppliers) { // NOLINT
  // Exercise system
  auto actual_attr_suppliers = model->getAttributeSuppliers();

  // Verify results
  ASSERT_EQ(attr_suppliers, actual_attr_suppliers);
}

void AttributeCollectionModelTest::checkCreateAttributeModel(
    std::shared_ptr<MockAttributeModel> attr_model,
    SlotCatcher<IAttributeModel::ValidateCompleteSlotType>* validateComplete) {
  (void)validateComplete;
  // Setup fixture
  IAttribute* attr = xtestutils::genDummyPointer<IAttribute>();
  std::shared_ptr<utils::ITrackable> null_trackobj;

  // Expectations
  EXPECT_CALL(attr_model_factory, createModel(attr))
      .WillOnce(Return(attr_model));
  EXPECT_CALL(*attr_model, whenValidateComplete(_, null_trackobj))
      .WillOnce(SaveArg<0>(validateComplete));

  // Exercise system
  auto actual_attr_model = model->createAttributeModel(attr);

  // Verify results
  ASSERT_EQ(attr_model, actual_attr_model);
}

TEST_F(AttributeCollectionModelTest,
       should_be_able_to_create_AttributeModel_for_attr) { // NOLINT
  auto attr_model = std::make_shared<MockAttributeModel>();
  SlotCatcher<IAttributeModel::ValidateCompleteSlotType> validateComplete;

  CUSTOM_ASSERT(checkCreateAttributeModel(attr_model, &validateComplete));
}

TEST_F(AttributeCollectionModelTest,
       test_validate_complete) { // NOLINT
  // Setup fixture
  auto attr_model1 = std::make_shared<MockAttributeModel>();
  SlotCatcher<IAttributeModel::ValidateCompleteSlotType> validateComplete1;
  bool attr_model1_valid = true;

  CUSTOM_ASSERT(checkCreateAttributeModel(attr_model1, &validateComplete1));

  auto attr_model2 = std::make_shared<MockAttributeModel>();
  SlotCatcher<IAttributeModel::ValidateCompleteSlotType> validateComplete2;
  bool attr_model2_valid = true;

  CUSTOM_ASSERT(checkCreateAttributeModel(attr_model2, &validateComplete2));

  auto attr_model3 = std::make_shared<MockAttributeModel>();
  SlotCatcher<IAttributeModel::ValidateCompleteSlotType> validateComplete3;
  bool attr_model3_valid = true;

  CUSTOM_ASSERT(checkCreateAttributeModel(attr_model3, &validateComplete3));

  EXPECT_CALL(*attr_model1, isValid())
      .WillRepeatedly(Invoke([&attr_model1_valid]{
            return attr_model1_valid;
          }));

  EXPECT_CALL(*attr_model2, isValid())
      .WillRepeatedly(Invoke([&attr_model2_valid]{
            return attr_model2_valid;
          }));

  EXPECT_CALL(*attr_model3, isValid())
      .WillRepeatedly(Invoke([&attr_model3_valid]{
            return attr_model3_valid;
          }));

  auto mock_listener = MockListener::attachTo(model.get());

  // should signal false when a attr_model report validate false
  EXPECT_CALL(*mock_listener, ValidateComplete(false));

  attr_model2_valid = false;
  validateComplete2();

  Mock::VerifyAndClearExpectations(mock_listener.get());

  // still false when another attr_model report validate fals
  EXPECT_CALL(*mock_listener, ValidateComplete(false));
  attr_model3_valid = false;
  validateComplete3();

  Mock::VerifyAndClearExpectations(mock_listener.get());

  // still false when only one of the two invalid attr_model reports valid
  EXPECT_CALL(*mock_listener, ValidateComplete(false));
  attr_model2_valid = true;
  validateComplete2();

  Mock::VerifyAndClearExpectations(mock_listener.get());

  // should signal true when all attr_model becomes valid
  EXPECT_CALL(*mock_listener, ValidateComplete(true));
  attr_model3_valid = true;
  validateComplete3();
}

TEST_F(AttributeCollectionModelTest,
       test_switch_mode) { // NOLINT
  // initially in display mode

  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, SwitchToEditMode());
  model->switchMode();

  Mock::VerifyAndClearExpectations(mock_listener.get());

  EXPECT_CALL(*mock_listener, SwitchToDisplayMode());
  model->switchMode();

  Mock::VerifyAndClearExpectations(mock_listener.get());
}

}  // namespace tests
}  // namespace snailcore
