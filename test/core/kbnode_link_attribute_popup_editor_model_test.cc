//-*- TestCaseName: KbNodeLinkAttributePopupEditorModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/kbnode_link_attribute_popup_editor_model.h"
#include "core/mock_kbnode_link_attribute.h"
#include "core/mock_kbnode_attribute.h"
#include "snail/mock_attribute_model.h"
#include "snail/mock_attribute_set_model.h"
#include "core/mock_attribute_model_factory.h"
#include "core/mock_attribute_set_model_factory.h"
#include "core/mock_link_type.h"

namespace snailcore {
namespace tests {

class KbNodeLinkAttributePopupEditorModelTest : public ::testing::Test {
 public:
  KbNodeLinkAttributePopupEditorModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeLinkAttributePopupEditorModelTest() { }
  void SetUp() override {
    RECORD_USED_MOCK_OBJECTS_SETUP;

    MockKbNodeAttribute value_attr;
    R_EXPECT_CALL(link_attr, valueAttr()).WillOnce(Return(&value_attr));

    value_attr_copy = new MockKbNodeAttribute();
    R_EXPECT_CALL(value_attr, clone()).WillOnce(Return(value_attr_copy));

    // setup link types
    R_EXPECT_CALL(link_attr, protoLinkType())
        .WillOnce(Return(&proto_link_type));

    MockLinkType link_type;
    R_EXPECT_CALL(link_attr, linkType()).WillOnce(Return(&link_type));

    link_type_copy = new MockLinkType();
    R_EXPECT_CALL(link_type, clone())
        .WillOnce(Return(link_type_copy));

    model = utils::make_unique<KbNodeLinkAttributePopupEditorModel>(
        &link_attr, &attr_model_factory, &attr_set_model_factory);

    VERIFY_RECORDED_MOCK_OBJECTS;

    EXPECT_CALL(link_attr, supplier())
        .WillRepeatedly(Return(&link_attr_supplier));
  }
  // void TearDown() override { }

  // region: objects test subject depends on
  MockKbNodeLinkAttribute link_attr;
  MockKbNodeLinkAttributeSupplier link_attr_supplier;
  MockLinkType proto_link_type;

  // will release by model
  MockKbNodeAttribute* value_attr_copy;
  MockLinkType* link_type_copy;

  MockAttributeModelFactory attr_model_factory;
  MockAttributeSetModelFactory attr_set_model_factory;
  // endregion

  // region: test subject
  std::unique_ptr<KbNodeLinkAttributePopupEditorModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

class CreateAttrModelFixture : public TestFixture {
 public:
  CreateAttrModelFixture(const utils::U8String& name,
                            KbNodeLinkAttributePopupEditorModelTest* test_case)
      : TestFixture(name) {
    attr_model = std::make_shared<MockAttributeModel>();
    R_EXPECT_CALL(test_case->attr_model_factory,
                createAttributeModel(test_case->value_attr_copy))
        .WillOnce(Return(attr_model));

    R_EXPECT_CALL(*attr_model, whenValidateComplete(_, _))
        .WillOnce(SaveArg<0>(&validateComplete));
  }

  std::shared_ptr<MockAttributeModel> attr_model;

  using ValidateCompleteSlotType =
      IAttributeModel::ValidateCompleteSlotType;
  SlotCatcher<ValidateCompleteSlotType> validateComplete;
};

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_create_attr_model_for_value_attr) { // NOLINT
  // Setup fixture
  FixtureHelper(CreateAttrModelFixture, fixture)

  // Exercise system
  auto actual_attr_model = model->createValueAttrModel();

  // Verify results
  ASSERT_EQ(fixture.attr_model, actual_attr_model);
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_get_link_type_item_provider) { // NOLINT
  // Setup fixture
  auto link_type_item_provider =
      xtestutils::genDummyPointer<ITreeItemProvider>();
  EXPECT_CALL(link_attr_supplier, getLinkTypeItemProvider())
      .WillOnce(Return(link_type_item_provider));

  // Exercise system
  auto actual_item_provider = model->getLinkTypeItemProvider();

  // Verify results
  ASSERT_EQ(link_type_item_provider, actual_item_provider);
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_get_current_proto_link_type) { // NOLINT
  // Exercise system
  auto actual_proto_link_type = model->getCurrentProtoLinkType();

  // Verify results
  ASSERT_EQ(&proto_link_type, actual_proto_link_type);

  auto actual_proto_link_type_again = model->getCurrentProtoLinkType();
  ASSERT_EQ(&proto_link_type, actual_proto_link_type_again);
}

class CreateAttrSetModelFixture : public TestFixture {
 public:
  CreateAttrSetModelFixture(const utils::U8String& name,
                            KbNodeLinkAttributePopupEditorModelTest* test_case)
      : TestFixture(name) {
    auto attr_suppliers = createDummyAttrSuppliers();
    R_EXPECT_CALL(*(test_case->link_type_copy), attributeSuppliers())
        .WillOnce(Return(attr_suppliers));

    attr_set_model = std::make_shared<MockAttributeSetModel>();
    R_EXPECT_CALL(test_case->attr_set_model_factory,
                  createAttributeSetModel(attr_suppliers))
        .WillOnce(Return(attr_set_model));

    R_EXPECT_CALL(*attr_set_model, whenValidateComplete(_, _))
        .WillOnce(SaveArg<0>(&validateComplete));
  }

 public:
  std::shared_ptr<MockAttributeSetModel> attr_set_model;

  using ValidateCompleteSlotType =
      IAttributeSetModel::ValidateCompleteSlotType;
  SlotCatcher<ValidateCompleteSlotType> validateComplete;

 private:
  std::vector<IAttributeSupplier*> createDummyAttrSuppliers() {
    std::vector<IAttributeSupplier*> attr_suppliers;
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());

    return attr_suppliers;
  }
};

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_create_attribute_for_current_link_type_sub_attributes) { // NOLINT
  // Setup fixture
  FixtureHelper(CreateAttrSetModelFixture, fixture);

  // Exercise system
  auto actual_attr_set_model = model->getCurrentLinkAttrSetModel();

  // Verify results
  ASSERT_EQ(fixture.attr_set_model, actual_attr_set_model);
}

BEGIN_MOCK_LISTENER_DEF(MockListener, IKbNodeLinkAttributePopupEditorModel)

MOCK_METHOD2(LinkTypeChanged,
             void(std::shared_ptr<IAttributeSetModel> new_attr_set_model,
                  IAttributeSetModel* old_attr_set_model));
MOCK_METHOD1(ValidateComplete, void(bool result));

BEGIN_BIND_SIGNAL(IKbNodeLinkAttributePopupEditorModel)

BIND_SIGNAL2(LinkTypeChanged,
             void, std::shared_ptr<IAttributeSetModel>, new_attr_set_model,
             IAttributeSetModel*, old_attr_set_model);
BIND_SIGNAL1(ValidateComplete, void, bool, result);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_make_a_copy_when_set_proto_link_type_and_fire_link_type_changed_event) { // NOLINT
  // Setup fixture
  std::shared_ptr<IAttributeSetModel> old_attr_set_model;
  {
    FixtureHelper(CreateAttrSetModelFixture, fixture);
    old_attr_set_model = model->getCurrentLinkAttrSetModel();
  }

  MockLinkType new_proto_link_type;
  MockLinkType* proto_link_type_copy = new MockLinkType();
  EXPECT_CALL(new_proto_link_type, clone())
      .WillOnce(Return(proto_link_type_copy));

  link_type_copy = proto_link_type_copy;

  FixtureHelper(CreateAttrSetModelFixture, create_attr_model_fixture);
  std::shared_ptr<IAttributeSetModel> new_attr_set_model =
      create_attr_model_fixture.attr_set_model;

  // Expectations
  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener,
              LinkTypeChanged(new_attr_set_model, old_attr_set_model.get()));

  // Exercise system
  model->setProtoLinkType(&new_proto_link_type);

  // Verify results
  auto curr_link_type = model->getCurrentProtoLinkType();
  ASSERT_EQ(&new_proto_link_type, curr_link_type);
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_be_able_to_report_validate_complete_status) { // NOLINT
  // Setup fixture
  FixtureHelper(CreateAttrModelFixture, create_attr_model_fixture);
  model->createValueAttrModel();

  FixtureHelper(CreateAttrSetModelFixture, create_attr_set_model_fixture);
  model->getCurrentLinkAttrSetModel();

  // initially all are valid
  bool attr_model_valid = true;
  bool attr_set_model_valid = true;

  auto attr_model = create_attr_model_fixture.attr_model;
  EXPECT_CALL(*attr_model, isValid()).
      WillRepeatedly(Invoke(
                  [&attr_model_valid]() {
                    return attr_model_valid;
                  }));

  auto & attrModelValidateComplete =
      create_attr_model_fixture.validateComplete;
  auto & attrSetModelValidateComplete =
      create_attr_set_model_fixture.validateComplete;

  auto mock_listener = MockListener::attachTo(model.get());

  // Verify results
  {  // NOTE: actually there's 8 different cases, but the following is enough
    InSequence seq;

    attr_model_valid = true;
    attr_set_model_valid = false;  // changed
    EXPECT_CALL(*mock_listener, ValidateComplete(false));
    attrSetModelValidateComplete(attr_set_model_valid);

    attr_model_valid = false;  // changed
    attr_set_model_valid = false;
    EXPECT_CALL(*mock_listener, ValidateComplete(false));
    attrModelValidateComplete();

    attr_model_valid = false;
    attr_set_model_valid = true;  // changed
    EXPECT_CALL(*mock_listener, ValidateComplete(false));
    attrSetModelValidateComplete(attr_set_model_valid);

    attr_model_valid = true;  // changed
    attr_set_model_valid = true;
    EXPECT_CALL(*mock_listener, ValidateComplete(true));
    attrModelValidateComplete();
  }
}

TEST_F(KbNodeLinkAttributePopupEditorModelTest,
       should_sync_local_value_attr_and_link_type_copy_to_link_attr_when_edit_finished) { // NOLINT
  // Expectations
  EXPECT_CALL(link_attr, setValueAttr(Ref(*value_attr_copy)));
  EXPECT_CALL(link_attr, setProtoLinkType(&proto_link_type));
  EXPECT_CALL(link_attr, setLinkType(Ref(*link_type_copy)));

  // Exercise system
  model->editFinished();
}

}  // namespace tests
}  // namespace snailcore
