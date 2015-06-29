//-*- TestCaseName: AttributeAdderModelTest;-*-
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

#include "src/core/attribute_adder_model.h"
#include "snail/mock_attribute_container.h"
#include "src/utils/utils.h"
#include "utils/i18n.h"
#include "snail/mock_attribute.h"
#include "snail/mock_attribute_editor_model_factory.h"
#include "snail/mock_attribute_editor_model.h"

namespace snailcore {
namespace tests {

class AttributeAdderModelTest : public ::testing::Test {
 protected:
  AttributeAdderModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeAdderModelTest() { }
  virtual void SetUp() {
    setupAttributeContainer();
    model = std::make_shared<AttributeAdderModel>(&attr_container,
                                                  &attr_model_factory);
  }
  // virtual void TearDown() { }

  void setupAttributeContainer();
  void initDefaultAttributeEditorModel();
  void expectationsOnUpdateCurrentAttributeEditorModel(
      std::function<void()> updateFunc,
      int new_attr_index,
      bool initial_create);
  void assertCanRelayValidateCompleteSignalByAttrModel();

  // region: objects test subject depends on
  int default_attr_index { -1 };
  std::vector<IAttribute*> allowed_attr_list;
  std::vector<MockAttribute*> allowed_mock_attr_list;
  MockAttributeContainer attr_container;

  MockAttributeEditorModelFactory attr_model_factory;
  MockObjectGenerator<MockAttribute> mock_attr_generator;
  // endregion

  // region: test subject
  std::shared_ptr<IAttributeAdderModel> model;
  // endregion

  // region: object depends on test subject
  SlotCatcher<IAttributeEditorModel::ValidateCompleteSlotType>
  attrModelValidateComplete;
  // endregion
};

BEGIN_MOCK_LISTENER_DEF(MockListener, IAttributeAdderModel)

MOCK_METHOD1(ValidateComplete, void(bool result));
MOCK_METHOD1(DiscardAttributeEditorModel, void(IAttributeEditorModel*));
MOCK_METHOD1(CurrentAttributeEditorModelChanged,
             void(std::shared_ptr<IAttributeEditorModel>));

BEGIN_BIND_SIGNAL(IAttributeAdderModel)

BIND_SIGNAL1(ValidateComplete, void, bool, result);
BIND_SIGNAL1(DiscardAttributeEditorModel,
             void, IAttributeEditorModel*, attr_model);
BIND_SIGNAL1(CurrentAttributeEditorModelChanged,
             void, std::shared_ptr<IAttributeEditorModel>, attr_model);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()

void AttributeAdderModelTest::setupAttributeContainer() {
  // we are really only need some pointers, so its self to destroy after this
  // method return

  const int NUM_ATTRS = 3;

  for (int i = 0; i < NUM_ATTRS; ++i) {
    MockAttribute* attr = mock_attr_generator.generate();
    allowed_attr_list.push_back(attr);
    allowed_mock_attr_list.push_back(attr);
  }

  default_attr_index = std::rand() % allowed_attr_list.size();

  ON_CALL(attr_container, getAllowedAttributeList())
      .WillByDefault(Return(allowed_attr_list));
  ON_CALL(attr_container, getDefaultAllowedAttribute())
      .WillByDefault(Return(allowed_attr_list[default_attr_index]));

  auto container_desc = xtestutils::genRandomString();
  ON_CALL(attr_container, getDescription())
      .WillByDefault(Return(container_desc));
}

TEST_F(AttributeAdderModelTest,
       should_be_able_to_return_prompt_string) { // NOLINT
  // Expectations
  utils::U8String expect_prompt =
      utils::formatString(_(AttributeAdderModel::prompt_format_str),
                          attr_container.getDescription());

  // Exercise system
  auto actual_prompt = model->getPrompt();

  // Verify results
  ASSERT_EQ(expect_prompt, actual_prompt);
}

TEST_F(AttributeAdderModelTest,
       should_be_able_to_return_allowed_attr_list_of_attr_container) { // NOLINT
  // Exercise system
  auto actual_attr_list = model->getAllowedAttributeList();

  // Verify results
  ASSERT_EQ(allowed_attr_list, actual_attr_list);
}

TEST_F(AttributeAdderModelTest,
       should_intially_return_the_index_of_default_attr_of_attribute_container) { // NOLINT
  // Verify results
  ASSERT_EQ(default_attr_index, model->getCurrentAttributeIndex());
}

void
AttributeAdderModelTest::assertCanRelayValidateCompleteSignalByAttrModel() {
  auto tester = [this](bool expect_result) {
    // Expectations
    auto mock_listener = MockListener::attachTo(model.get());
    EXPECT_CALL(*mock_listener, ValidateComplete(expect_result));

    // Exercise system
    attrModelValidateComplete(expect_result);
  };

  tester(true);
  tester(false);
}

void AttributeAdderModelTest::expectationsOnUpdateCurrentAttributeEditorModel(
    std::function<void()> updateFunc,
    int new_attr_index, bool initial_create) {
  // Setup fixture
  auto attr_prototype = allowed_mock_attr_list[new_attr_index];
  MockAttribute new_attr;
  auto new_attr_model = std::make_shared<MockAttributeEditorModel>();

  RECORD_USED_MOCK_OBJECTS_SETUP;

  // Expectations
  R_EXPECT_CALL(*attr_prototype, clone()).WillOnce(Return(&new_attr));
  R_EXPECT_CALL(attr_model_factory, createAttributeEditorModel(&new_attr))
      .WillOnce(Return(new_attr_model));
  R_EXPECT_CALL(*new_attr_model, whenValidateComplete(_, _))
      .WillOnce(SaveArg<0>(&attrModelValidateComplete));

  auto mock_listener = MockListener::attachTo(model.get());
  if (!initial_create) {
    InSequence seq;

    auto curr_attr_model = model->getCurrentAttributeEditorModel().get();
    R_EXPECT_CALL(*mock_listener, DiscardAttributeEditorModel(curr_attr_model));
    std::shared_ptr<IAttributeEditorModel> new_attr_imodel = new_attr_model;
    R_EXPECT_CALL(*mock_listener,
                  CurrentAttributeEditorModelChanged(new_attr_imodel));
  }

  // Exercise system
  updateFunc();

  // Verify results
  ASSERT_EQ(new_attr_model, model->getCurrentAttributeEditorModel());
  ASSERT_EQ(new_attr_index, model->getCurrentAttributeIndex());
  VERIFY_RECORDED_MOCK_OBJECTS;
  CUSTOM_ASSERT(assertCanRelayValidateCompleteSignalByAttrModel());
}

void AttributeAdderModelTest::initDefaultAttributeEditorModel() {
  expectationsOnUpdateCurrentAttributeEditorModel([this]() {
      model->getCurrentAttributeEditorModel();
    }, default_attr_index, true);
}

TEST_F(AttributeAdderModelTest,
       should_create_AttrModel_for_default_attr_when_getCurrentAttributeEditorModel_initially_called) { // NOLINT
  CUSTOM_ASSERT(initDefaultAttributeEditorModel());
}

TEST_F(AttributeAdderModelTest,
       should_replace_current_attr_model_when_user_select_a_different_attr) { // NOLINT
  // Setup fixture
  CUSTOM_ASSERT(initDefaultAttributeEditorModel());

  int new_index = (model->getCurrentAttributeIndex() + 1)
                  % allowed_attr_list.size();

  expectationsOnUpdateCurrentAttributeEditorModel(
      [this, new_index]() {
        model->setCurrentAttributeIndex(new_index);
      }, new_index, false);
}

TEST_F(AttributeAdderModelTest,
       should_do_nothing_if_user_select_the_same_index_stored_in_model) { // NOLINT
  // Setup fixture
  CUSTOM_ASSERT(initDefaultAttributeEditorModel());

  auto curr_attr_index = model->getCurrentAttributeIndex();
  auto curr_attr_prototype = allowed_mock_attr_list[curr_attr_index];
  auto saved_attr_model = model->getCurrentAttributeEditorModel();
  auto curr_mock_attr_model =
      std::dynamic_pointer_cast<MockAttributeEditorModel>(saved_attr_model);

  // Expectations
  EXPECT_CALL(*curr_attr_prototype, clone()).Times(0);
  EXPECT_CALL(attr_model_factory, createAttributeEditorModel(_)).Times(0);
  EXPECT_CALL(*curr_mock_attr_model, whenValidateComplete(_, _)).Times(0);

  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, DiscardAttributeEditorModel(_)).Times(0);
  EXPECT_CALL(*mock_listener, CurrentAttributeEditorModelChanged(_)).Times(0);

  // Exercise system
  model->setCurrentAttributeIndex(curr_attr_index);

  // Verify results
  ASSERT_EQ(saved_attr_model, model->getCurrentAttributeEditorModel());
  ASSERT_EQ(curr_attr_index, model->getCurrentAttributeIndex());
}

TEST_F(AttributeAdderModelTest,
       should_add_current_attribute_to_attribute_container_when_user_clicked_add) { // NOLINT
  // Setup fixture
  CUSTOM_ASSERT(initDefaultAttributeEditorModel());

  auto curr_attr_model = model->getCurrentAttributeEditorModel();
  auto curr_mock_attr_model =
      std::dynamic_pointer_cast<MockAttributeEditorModel>(curr_attr_model);
  MockAttribute attr;

  // Expectations
  EXPECT_CALL(*curr_mock_attr_model, getAttribute()).WillOnce(Return(&attr));
  EXPECT_CALL(attr_container, addAttribute(&attr));

  // Exercise system
  expectationsOnUpdateCurrentAttributeEditorModel([this](){
      model->doAddAttribute();
    },
    model->getCurrentAttributeIndex(),
    false);
}

TEST_F(AttributeAdderModelTest,
       attribute_validate_is_retrieved_from_AttributeEditorModel) { // NOLINT
  // Setup fixture
  CUSTOM_ASSERT(initDefaultAttributeEditorModel());

  auto curr_attr_model = model->getCurrentAttributeEditorModel();
  auto curr_mock_attr_model =
      std::dynamic_pointer_cast<MockAttributeEditorModel>(curr_attr_model);
  auto raw_attr_model = curr_mock_attr_model.get();

  auto tester = [this, raw_attr_model](bool expect_result) {
    // Expectations
    EXPECT_CALL(*raw_attr_model, validateResult())
    .WillOnce(Return(expect_result));

    // Excercise system
    bool actual_result = model->validateResult();

    // Verify results
    ::Mock::VerifyAndClear(raw_attr_model);
    ASSERT_EQ(expect_result, actual_result);
  };

  tester(true);
  tester(false);
}

}  // namespace tests
}  // namespace snailcore
