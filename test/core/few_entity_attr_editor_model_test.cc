//-*- TestCaseName: FewEntityAttrEditorModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/few_entity_attr_editor_model.h"
#include "snail/mock_attribute.h"
#include "snail/mock_entity_provider.h"
#include "test/core/entity_test_helper.h"
#include "core/mock_entity_candidate_item_converter.h"
#include "snail/candidate_item.h"

namespace snailcore {
namespace tests {

class FewEntityAttrEditorModelTest : public ::testing::Test {
 protected:
  FewEntityAttrEditorModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~FewEntityAttrEditorModelTest() { }
  virtual void SetUp() {
    setupEntityProvider();

    curr_entity = std::make_shared<MockEntity>();
    std::shared_ptr<const IEntity> default_ientity = curr_entity;
    ON_CALL(entity_provider, getDefaultEntity())
        .WillByDefault(Return(default_ientity));

    auto l_entity_item_converter =
        utils::make_unique<MockEntityCandidateItemConverter>();
    entity_item_converter = l_entity_item_converter.get();

    RECORD_USED_MOCK_OBJECTS_SETUP;

    // init defaults
    R_EXPECT_CALL(backing_attr, setEntity(default_ientity));

    model = std::make_shared<FewEntityAttrEditorModel>(
        &backing_attr,
        &entity_provider,
        std::move(l_entity_item_converter));

    VERIFY_RECORDED_MOCK_OBJECTS;

    ON_CALL(backing_attr, getEntity())
        .WillByDefault(Return(curr_entity));
    ASSERT_TRUE(model->validateResult());
  }
  // virtual void TearDown() { }

  void setupEntityProvider();

  // region: objects test subject depends on
  MockAttribute backing_attr;
  std::vector<std::shared_ptr<const IEntity>> entities;
  std::shared_ptr<MockEntity> curr_entity;
  MockEntityProvider entity_provider;
  MockEntityCandidateItemConverter* entity_item_converter { nullptr };
  // endregion

  // region: test subject
  std::shared_ptr<IFewEntityAttrEditorModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

BEGIN_MOCK_LISTENER_DEF(MockListener, IFewEntityAttrEditorModel)

MOCK_METHOD1(ValidateComplete, void(bool result));

BEGIN_BIND_SIGNAL(IFewEntityAttrEditorModel)

BIND_SIGNAL1(ValidateComplete, void, bool, result);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()


TEST_F(FewEntityAttrEditorModelTest,
       test_when_there_is_no_default_enitity) { // NOLINT
  // Expectations
  ON_CALL(entity_provider, getDefaultEntity())
      .WillByDefault(Return(nullptr));
  ON_CALL(backing_attr, getEntity())
      .WillByDefault(Return(nullptr));

  EXPECT_CALL(backing_attr, setEntity(_)).Times(0);

  // Exercise system
  auto another_model = std::make_shared<FewEntityAttrEditorModel>(
      &backing_attr, &entity_provider, nullptr);

  // Verify results
  ASSERT_FALSE(another_model->validateResult());
}

TEST_F(FewEntityAttrEditorModelTest,
       should_be_able_to_get_back_the_backing_attr) { // NOLINT
  ASSERT_EQ(&backing_attr, model->getAttribute());
}

void FewEntityAttrEditorModelTest::setupEntityProvider() {
  entities = EntityTestHelper::genEntities();
  ON_CALL(entity_provider, getEntities())
      .WillByDefault(Return(entities));
}

TEST_F(FewEntityAttrEditorModelTest,
       should_be_able_to_get_candidate_entities_and_convert_to_candidte_items) { // NOLINT
  // Setup fixture
  auto expect_root_item = xtestutils::genDummyPointer<CandidateItem>();

  // Expectations
  EXPECT_CALL(*entity_item_converter, toCandidateItems(entities))
      .WillOnce(Return(expect_root_item));

  // Verify results
  auto actual_root_item = model->getCandidateEntities();
  ASSERT_EQ(expect_root_item, actual_root_item);
}

TEST_F(FewEntityAttrEditorModelTest,
       should_be_able_to_get_current_entity_name) { // NOLINT
  // Setup fixture
  auto entity_name = xtestutils::genRandomString();

  // Expectations
  EXPECT_CALL(*curr_entity, name())
      .WillOnce(Return(entity_name));

  // Exercise system
  auto actual_name = model->getCurrentEntityName();

  // Verify results
  ASSERT_EQ(entity_name, actual_name);
}

TEST_F(FewEntityAttrEditorModelTest,
       should_return_empty_string_when_curr_attr_has_no_entity_setted) { // NOLINT
  // Setup fixture
  ON_CALL(backing_attr, getEntity())
      .WillByDefault(Return(nullptr));

  utils::U8String empty_string { "" };

  // Exercise system
  auto actual_name = model->getCurrentEntityName();

  // Verify results
  ASSERT_EQ(empty_string, actual_name);
}

TEST_F(FewEntityAttrEditorModelTest,
       should_set_user_selected_entity_to_backing_attr) { // NOLINT
  // Setup fixture
  auto entity = std::make_shared<MockEntity>();
  std::shared_ptr<const IEntity> ientity = entity;
  CandidateItem item;

  // Expectations
  EXPECT_CALL(*entity_item_converter, toEntity(Ref(item)))
      .WillOnce(Return(entity));

  auto mock_listener = MockListener::attachTo(model.get());
  {
    InSequence seq;

    EXPECT_CALL(backing_attr, setEntity(ientity));

    EXPECT_CALL(entity_provider, touchEntity(ientity));

    EXPECT_CALL(*mock_listener, ValidateComplete(true));
  }

  // Exercise system
  model->setCurrentEntity(item);
}

}  // namespace tests
}  // namespace snailcore
