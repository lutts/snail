//-*- TestCaseName: FewEntityAttrEditorModelTest;-*-
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

#include "src/core/few_entity_attr_editor_model.h"
#include "snail/mock_attribute.h"
#include "snail/mock_entity_provider.h"
#include "test/core/entity_test_helper.h"

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

    auto default_entity = std::make_shared<MockEntity>();
    std::shared_ptr<const IEntity> default_ientity = default_entity;
    ON_CALL(entity_provider, getDefaultEntity())
        .WillByDefault(Return(default_entity));

    RECORD_USED_MOCK_OBJECTS_SETUP;

    // init defaults
    R_EXPECT_CALL(backing_attr, setEntity(default_ientity));

    model = std::make_shared<FewEntityAttrEditorModel>(
        &backing_attr,
        &entity_provider);

    VERIFY_RECORDED_MOCK_OBJECTS;

    ON_CALL(backing_attr, getEntity())
        .WillByDefault(Return(default_entity));
    ASSERT_TRUE(model->validateResult());
  }
  // virtual void TearDown() { }

  void setupEntityProvider();

  // region: objects test subject depends on
  MockAttribute backing_attr;
  std::vector<std::shared_ptr<const IEntity>> entities;
  MockEntityProvider entity_provider;
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
      &backing_attr, &entity_provider);

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
         candidate_entities_is_retrieved_from_entity_provider) { // NOLINT
  // Exercise system
  auto actual_entities = model->getCandidateEntities();

  // Verify results
  ASSERT_EQ(entities, actual_entities);
}

TEST_F(FewEntityAttrEditorModelTest,
       should_setCurrentEntity_set_the_entity_to_backing_attr) { // NOLINT
  // Setup fixture
  auto entity = std::make_shared<MockEntity>();
  std::shared_ptr<const IEntity> ientity = entity;

  // Expectations
  EXPECT_CALL(backing_attr, setEntity(ientity));
  EXPECT_CALL(entity_provider, touchEntity(ientity));

  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, ValidateComplete(true));

  // Exercise system
  model->setCurrentEntity(entity);
}

TEST_F(FewEntityAttrEditorModelTest,
       should_current_index_be_minus_1_when_backing_attr_has_no_entity_setted) { // NOLINT
  // Setup fixture
  ON_CALL(backing_attr, getEntity())
      .WillByDefault(Return(nullptr));

  int expect_index = -1;

  // Exercise system
  auto actual_index = model->getCurrentEntityIndex();

  // Verify results
  ASSERT_EQ(expect_index, actual_index);
}

TEST_F(FewEntityAttrEditorModelTest,
       should_current_index_be_index_of_backing_attr_entity_in_all_entities) { // NOLINT
  // Setup fixture
  int expect_index = std::rand() % entities.size();
  ON_CALL(backing_attr, getEntity())
      .WillByDefault(Return(entities[expect_index]));

  // Exercise system
  auto actual_index = model->getCurrentEntityIndex();

  // Verify results
  ASSERT_EQ(expect_index, actual_index);
}

}  // namespace tests
}  // namespace snailcore
