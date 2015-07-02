//-*- TestCaseName: FewEntityAttrEditorPresenterTest;-*-
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

// triad headers
#include "snail/mock_few_entity_attr_editor_model.h"
#include "qtui/mock_few_entity_attr_editor_view.h"
#include "src/qtui/few_entity_attr_editor_presenter.h"

#include "snail/mock_entity.h"
#include "snail/candidate_item.h"
#include "qtui/mock_candidate_item_qmodel_adapter.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

class FewEntityAttrEditorPresenterTest : public ::testing::Test {
 protected:
  FewEntityAttrEditorPresenterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~FewEntityAttrEditorPresenterTest() { }
  virtual void SetUp() {
    model = std::make_shared<MockFewEntityAttrEditorModel>();
    view = std::make_shared<MockFewEntityAttrEditorView>();

    auto entity_candidate_adapter =
        utils::make_unique<MockCandidateItemQModelAdapter>();

    CandidateItem candidate_root_item;

    RECORD_USED_MOCK_OBJECTS_SETUP;

    // init selection list && default
    {
      InSequence seq;

      R_EXPECT_CALL(*model, getCandidateEntities())
          .WillOnce(Return(&candidate_root_item));
      R_EXPECT_CALL(*entity_candidate_adapter,
                    setCandidates(Ref(candidate_root_item)));
      R_EXPECT_CALL(*view,
                    setEntitySelectorQModel(entity_candidate_adapter.get()));

      auto default_entity_name = xtestutils::genRandomString();

      R_EXPECT_CALL(*model, getCurrentEntityName())
          .WillOnce(Return(default_entity_name));
      R_EXPECT_CALL(*view, setCurrentEntityName(default_entity_name));
    }

    R_EXPECT_CALL(*view, whenEntitySelectionChanged(_, _))
        .WillOnce(SaveArg<0>(&entitySelectionChanged));

    presenter = std::make_shared<FewEntityAttrEditorPresenter>(
        model, view, std::move(entity_candidate_adapter));
    presenter->initialize();

    VERIFY_RECORDED_MOCK_OBJECTS;

  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  std::shared_ptr<MockFewEntityAttrEditorModel> model;
  std::shared_ptr<MockFewEntityAttrEditorView> view;
  // endregion

  // region: test subject
  std::shared_ptr<FewEntityAttrEditorPresenter> presenter;
  // endregion

  // region: object depends on test subject
  SlotCatcher<IFewEntityAttrEditorView::EntitySelectionChangedSlotType>
  entitySelectionChanged;
  // endregion
};

TEST_F(FewEntityAttrEditorPresenterTest,
       should_notify_model_when_current_selection_changed) { // NOLINT
  // Setup fixture
  CandidateItem entity_item;

  // Expectations
  EXPECT_CALL(*model, setCurrentEntity(Ref(entity_item)));

  // Exercise system
  entitySelectionChanged(&entity_item);
}
