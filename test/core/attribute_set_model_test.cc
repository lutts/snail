//-*- TestCaseName: AttributeSetModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "pfmvp/mock_pf_triad_manager.h"

#include "src/core/attribute_set_model.h"
#include "core/mock_attribute_model_factory.h"
#include "snail/mock_attribute_model.h"

#include "test/core/attribute_test_stub.h"
#include "snail/mock_attribute_supplier.h"

namespace snailcore {
namespace tests {

class AttributeSetModelTest : public ::testing::Test {
 protected:
  AttributeSetModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeSetModelTest() { }
  void SetUp() override {
    setupAttributeSuppliers();
    model = utils::make_unique<AttributeSetModel>(attr_suppliers,
                                                  &attr_model_factory);
    // default to display mode
    ASSERT_FALSE(model->isEditMode());
  }
  // void TearDown() override { }

  void setupAttributeSuppliers();
  void switchToEditMode();
  void switchToDisplayMode();
  void checkCreateAttributeModel(
      std::shared_ptr<MockAttributeModel> attr_model,
      SlotCatcher<IAttributeModel::ValidateCompleteSlotType>* validateComplete);

  // region: objects test subject depends on
  std::vector<std::unique_ptr<MockAttrSupplierTestStub> > attr_suppliers_up;
  std::vector<IAttributeSupplier*> attr_suppliers;
  MockAttributeModelFactory attr_model_factory;
  // endregion

  // region: test subject
  std::unique_ptr<AttributeSetModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

class MockListener : public SimpleMockListener<IAttributeSetModel> {
 public:
  SNAIL_MOCK_LISTENER0(MockListener, SwitchToEditMode, void());
  SNAIL_MOCK_LISTENER0(MockListener, SwitchToDisplayMode, void());
  SNAIL_MOCK_LISTENER1(MockListener, ValidateComplete,
                       void(bool validate_result));

  MockListener(IAttributeSetModel* model) : SimpleMockListener(model) {
    SNAIL_MOCK_LISTENER_REGISTER(SwitchToEditMode, this);
    SNAIL_MOCK_LISTENER_REGISTER(SwitchToDisplayMode, this);
    SNAIL_MOCK_LISTENER_REGISTER(ValidateComplete, this);

    attach();
  }

  ~MockListener() { detatch(); }
};

void AttributeSetModelTest::setupAttributeSuppliers() {
  attr_suppliers_up = MockAttrSupplierTestStub::createAttributeSuppliers();
  for (auto& supplier : attr_suppliers_up) {
    attr_suppliers.push_back(supplier.get());
  }
}

TEST_F(
    AttributeSetModelTest,
    should_be_able_to_get_the_suppliers_passed_when_construction) {  // NOLINT
  // Exercise system
  auto actual_attr_suppliers = model->getAttributeSuppliers();

  // Verify results
  ASSERT_EQ(attr_suppliers, actual_attr_suppliers);
}

void AttributeSetModelTest::switchToEditMode() {
  // Setup fixture
  auto max_1_attr_0_idx =
      MockAttrSupplierTestStub::kIdx_Supplier_Max_Eq_1_AttrCount_Eq_0;
  auto max_1_attr_0_supplier = attr_suppliers_up[max_1_attr_0_idx].get();

  // Expectations
  // use ComplexReturnValue to ensure only this supplier's addAttributed called
  EXPECT_CALL(*max_1_attr_0_supplier, addAttributeCalled(_))
      .WillOnce(Return(ComplexReturnValue(0)));

  MockListener mock_listener(model.get());
  EXPECT_CALL(mock_listener, SwitchToEditMode());
  EXPECT_CALL(mock_listener, SwitchToDisplayMode()).Times(0);

  // Exercise system
  model->switchMode();

  // Verify result
  ASSERT_TRUE(model->isEditMode());
  Mock::VerifyAndClearExpectations(max_1_attr_0_supplier);
}

TEST_F(AttributeSetModelTest, test_switch_to_edit_mode) {  // NOLINT
  CUSTOM_ASSERT(switchToEditMode());
}

TEST_F(AttributeSetModelTest, test_switch_to_display_mode) {  // NOLINT
  // Setup fixture
  switchToEditMode();

  // automatically added attr
  auto max_1_attr_0_idx =
      MockAttrSupplierTestStub::kIdx_Supplier_Max_Eq_1_AttrCount_Eq_0;
  auto max_1_attr_0_supplier = attr_suppliers_up[max_1_attr_0_idx].get();
  auto automatically_added_attr = max_1_attr_0_supplier->attributes()[0];

  // manually added attr, but remains empty when switch display mode
  auto max_gt_1_attr_0_idx =
      MockAttrSupplierTestStub::kIdx_Supplier_Max_Gt_1_AttrCount_Eq_0;
  auto max_gt_1_attr_0_supplier = attr_suppliers_up[max_gt_1_attr_0_idx].get();

  EXPECT_CALL(*max_gt_1_attr_0_supplier, addAttributeCalled(_))
      .WillOnce(Return(ComplexReturnValue(0)));
  auto newly_added_attr_empty = max_gt_1_attr_0_supplier->addAttribute();

  // mannually added attr, but has valid value setted
  auto max_gt_1_attr_eq_max_minus_2_idx =
      MockAttrSupplierTestStub::kIdx_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2;
  auto max_gt_1_attr_eq_max_minus_2_supplier =
      attr_suppliers_up[max_gt_1_attr_eq_max_minus_2_idx].get();
  EXPECT_CALL(*max_gt_1_attr_eq_max_minus_2_supplier, addAttributeCalled(_))
      .WillOnce(Return(ComplexReturnValue(0)));
  auto newly_added_attr_not_empty = static_cast<AttributeTestStub*>(
      max_gt_1_attr_eq_max_minus_2_supplier->addAttribute());
  newly_added_attr_not_empty->setValueText(xtestutils::genRandomString());

  // manually clear an attribute
  auto max_gt_1_attr_eq_max_idx =
      MockAttrSupplierTestStub::kIdx_Supplier_Max_Gt_1_AttrCount_Eq_Max;
  auto max_gt_1_attr_eq_max_supplier =
      attr_suppliers_up[max_gt_1_attr_eq_max_idx].get();
  auto cleared_attr = max_gt_1_attr_eq_max_supplier->attributes()[1];
  cleared_attr->clear();

  // Expectations
  EXPECT_CALL(*max_1_attr_0_supplier,
              attributeRemoved(automatically_added_attr))
      .WillOnce(Return(ComplexReturnValue(0)));

  EXPECT_CALL(*max_gt_1_attr_0_supplier,
              attributeRemoved(newly_added_attr_empty))
      .WillOnce(Return(ComplexReturnValue(0)));

  EXPECT_CALL(*max_gt_1_attr_eq_max_supplier, attributeRemoved(cleared_attr))
      .WillOnce(Return(ComplexReturnValue(0)));

  MockListener mock_listener(model.get());
  EXPECT_CALL(mock_listener, SwitchToDisplayMode());
  EXPECT_CALL(mock_listener, SwitchToEditMode()).Times(0);

  // Exercise system
  model->switchMode();
}

void AttributeSetModelTest::switchToDisplayMode() {
  auto max_1_attr_0_idx =
      MockAttrSupplierTestStub::kIdx_Supplier_Max_Eq_1_AttrCount_Eq_0;
  auto max_1_attr_0_supplier = attr_suppliers_up[max_1_attr_0_idx].get();
  EXPECT_CALL(*max_1_attr_0_supplier, attributeRemoved(_))
      .WillOnce(Return(ComplexReturnValue(0)));

  model->switchMode();

  ASSERT_FALSE(model->isEditMode());
  Mock::VerifyAndClearExpectations(max_1_attr_0_supplier);
}

void AttributeSetModelTest::checkCreateAttributeModel(
    std::shared_ptr<MockAttributeModel> attr_model,
    SlotCatcher<IAttributeModel::ValidateCompleteSlotType>* validateComplete) {
  // Setup fixture
  IAttribute* attr = xtestutils::genDummyPointer<IAttribute>();
  std::shared_ptr<utils::ITrackable> null_trackobj;

  // Expectations
  EXPECT_CALL(attr_model_factory, createAttributeModel(attr))
      .WillOnce(Return(attr_model));
  EXPECT_CALL(*attr_model, whenValidateComplete(_, null_trackobj))
      .WillOnce(SaveArg<0>(validateComplete));

  // Exercise system
  auto actual_attr_model = model->createAttributeModel(attr);

  // Verify results
  ASSERT_EQ(attr_model, actual_attr_model);
}

TEST_F(AttributeSetModelTest,
       should_be_able_to_create_AttributeModel_for_attr) {  // NOLINT
  auto attr_model = std::make_shared<MockAttributeModel>();
  SlotCatcher<IAttributeModel::ValidateCompleteSlotType> validateComplete;

  CUSTOM_ASSERT(checkCreateAttributeModel(attr_model, &validateComplete));
}

class AttributeSetModelTest_AttrEditorTriadTests
    : public AttributeSetModelTest {
 public:
  void SetUp() override {
    AttributeSetModelTest::SetUp();

    switchToEditMode();

    attr_model1 = std::make_shared<MockAttributeModel>();
    attr_model2 = std::make_shared<MockAttributeModel>();
    attr_model3 = std::make_shared<MockAttributeModel>();

    CUSTOM_ASSERT(checkCreateAttributeModel(attr_model1, &validateComplete1));
    CUSTOM_ASSERT(checkCreateAttributeModel(attr_model2, &validateComplete2));
    CUSTOM_ASSERT(checkCreateAttributeModel(attr_model3, &validateComplete3));
  }

  std::shared_ptr<MockAttributeModel> attr_model1;
  SlotCatcher<IAttributeModel::ValidateCompleteSlotType> validateComplete1;

  std::shared_ptr<MockAttributeModel> attr_model2;
  SlotCatcher<IAttributeModel::ValidateCompleteSlotType> validateComplete2;

  std::shared_ptr<MockAttributeModel> attr_model3;
  SlotCatcher<IAttributeModel::ValidateCompleteSlotType> validateComplete3;
};

TEST_F(AttributeSetModelTest_AttrEditorTriadTests,
       test_validate_complete) {  // NOLINT
  // Setup fixture
  bool attr_model1_valid = true;
  bool attr_model2_valid = true;
  bool attr_model3_valid = true;

  EXPECT_CALL(*attr_model1, isValid())
      .WillRepeatedly(
          Invoke([&attr_model1_valid] { return attr_model1_valid; }));

  EXPECT_CALL(*attr_model2, isValid())
      .WillRepeatedly(
          Invoke([&attr_model2_valid] { return attr_model2_valid; }));

  EXPECT_CALL(*attr_model3, isValid())
      .WillRepeatedly(
          Invoke([&attr_model3_valid] { return attr_model3_valid; }));

  MockListener mock_listener(model.get());

  // should signal false when a attr_model report validate false
  EXPECT_CALL(mock_listener, ValidateComplete(false));

  attr_model2_valid = false;
  (void)attr_model2_valid;  // make scan-build happy
  validateComplete2();

  Mock::VerifyAndClearExpectations(&mock_listener);

  // still false when another attr_model report validate fals
  EXPECT_CALL(mock_listener, ValidateComplete(false));
  attr_model3_valid = false;
  (void)attr_model3_valid;  // make scan-build happy
  validateComplete3();

  Mock::VerifyAndClearExpectations(&mock_listener);

  // still false when only one of the two invalid attr_model reports valid
  EXPECT_CALL(mock_listener, ValidateComplete(false));
  attr_model2_valid = true;
  (void)attr_model2_valid;  // make scan-build happy
  validateComplete2();

  Mock::VerifyAndClearExpectations(&mock_listener);

  // should signal true when all attr_model becomes valid
  EXPECT_CALL(mock_listener, ValidateComplete(true));
  attr_model3_valid = true;
  (void)attr_model3_valid;  // make scan-build happy
  validateComplete3();

  Mock::VerifyAndClearExpectations(&mock_listener);

  // test delete attr model
  attr_model2.reset();
  validateComplete3();
  validateComplete3();
}

TEST_F(AttributeSetModelTest_AttrEditorTriadTests,
       should_not_emit_validate_complete_in_display_mode) {  // NOLINT
  // Setup fixture
  switchToDisplayMode();

  // Expectations
  MockListener mock_listener(model.get());
  EXPECT_CALL(mock_listener, ValidateComplete(_)).Times(0);

  // Exercise system
  validateComplete1();
  validateComplete2();
  validateComplete3();
}

TEST_F(AttributeSetModelTest_AttrEditorTriadTests,
       should_be_able_to_close_attribute_editors) {  // NOLINT
  // Setup fixture
  pfmvp::tests::MockPfTriadManager triad_manager;

  // Expectations
  {
    InSequence seq;

    EXPECT_CALL(*attr_model1, cleanupValidateCompleteSlots());
    EXPECT_CALL(triad_manager, removeTriadBy(attr_model1.get()));
  }

  {
    InSequence seq;

    EXPECT_CALL(*attr_model2, cleanupValidateCompleteSlots());
    EXPECT_CALL(triad_manager, removeTriadBy(attr_model2.get()));
  }

  {
    InSequence seq;

    EXPECT_CALL(*attr_model3, cleanupValidateCompleteSlots());
    EXPECT_CALL(triad_manager, removeTriadBy(attr_model3.get()));
  }

  // Exercise system
  model->closeAttributeEditors(&triad_manager);

  // Verify result
  // after close editors, validate complete should do nothing
  MockListener mock_listener(model.get());
  EXPECT_CALL(mock_listener, ValidateComplete(_)).Times(0);

  validateComplete1();
  validateComplete2();
  validateComplete3();
}

}  // namespace tests
}  // namespace snailcore
