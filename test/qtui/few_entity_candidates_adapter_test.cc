//-*- TestCaseName: FewEntityCandidatesAdapterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QStringList>
#include <QStringListModel>

#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/utils.h"
#include "test/testutils/generic_mock_listener.h"
#include "test/testutils/slot_catcher.h"
#include "test/testutils/random_string.h"

#include "src/qtui/few_entity_candidates_adapter.h"
#include "qtui/mock_few_entity_attr_editor_view.h"
#include "snail/mock_entity_provider.h"
#include "test/core/entity_test_helper.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

class FewEntityCandidatesAdapterTest : public ::testing::Test {
 protected:
  FewEntityCandidatesAdapterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~FewEntityCandidatesAdapterTest() { }
  virtual void SetUp() {
    view = std::make_shared<MockFewEntityAttrEditorView>();

    RECORD_USED_MOCK_OBJECTS_SETUP;

    R_EXPECT_CALL(*view, whenEntitySelectionChanged(_, _))
        .WillOnce(SaveArg<0>(&entitySelectionChanged));

    adapter = FewEntityCandidatesAdapter::create(view);

    VERIFY_RECORDED_MOCK_OBJECTS;

    entities = EntityTestHelper::genEntities();
    QStringList qstr_list;

    for (auto& entity : entities) {
      qstr_list << U8StringToQString(entity->name());
    }

    expect_qstr_list_model.setStringList(qstr_list);
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  std::shared_ptr<MockFewEntityAttrEditorView> view;
  std::vector<std::shared_ptr<const IEntity>> entities;
  QStringListModel expect_qstr_list_model;
  int default_entity_index;

  MockEntityProvider entity_provider;
  // endregion

  // region: test subject
  std::shared_ptr<FewEntityCandidatesAdapter> adapter;
  // endregion

  // region: object depends on test subject
  SlotCatcher<IFewEntityAttrEditorView::EntitySelectionChangedSlotType>
  entitySelectionChanged;
  // endregion
};

BEGIN_MOCK_LISTENER_DEF(MockListener, IFewEntityCandidatesAdapter)

MOCK_METHOD1(EntitySelectionChanged,
               void(std::shared_ptr<const snailcore::IEntity> new_entity));

BEGIN_BIND_SIGNAL(IFewEntityCandidatesAdapter)

BIND_SIGNAL1(EntitySelectionChanged,
             void, std::shared_ptr<const snailcore::IEntity>, new_entity);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()

TEST_F(FewEntityCandidatesAdapterTest,
       should_generate_an_QStringListModel_and_set_to_view_when_configured_with_candidate_entities) { // NOLINT
  // Setup fixture
  QAbstractItemModel* actual_item_model = nullptr;

  {
    InSequence seq;

    EXPECT_CALL(*view, setEntitySelectorQModel(_))
        .WillOnce(SaveArg<0>(&actual_item_model));
  }

  // Exercise system
  adapter->setCandidateEntities(entities);

  // Verify result
  ASSERT_NE(nullptr, actual_item_model);
  auto actual_qstr_list_model =
      qobject_cast<QStringListModel*>(actual_item_model);
  ASSERT_NE(nullptr, actual_qstr_list_model);
  ASSERT_EQ(expect_qstr_list_model.stringList(),
            actual_qstr_list_model->stringList());
}

TEST_F(FewEntityCandidatesAdapterTest,
       should_fire_EntitySelectionChanged_with_user_selected_entity_id_when_user_select_another_entity) { // NOLINT
  // Setup fixture
  int index = std::rand() % entities.size();
  auto expect_entity = entities[index];

  adapter->setCandidateEntities(entities);

  // Expectations
  auto mock_listener = MockListener::attachTo(adapter.get());
  EXPECT_CALL(*mock_listener, EntitySelectionChanged(expect_entity));

  // Exercise system
  entitySelectionChanged(index);
}

TEST_F(FewEntityCandidatesAdapterTest,
       should_set_the_current_selection_index_to_view_when_requested) { // NOLINT
  // Setup fixture
  int index = std::rand();

  // Expectations
  EXPECT_CALL(*view, setCurrentEntityIndex(index));

  // Exercise system
  adapter->setCurrentEntityIndex(index);
}
