//-*- TestCaseName: CandidateItemQModelAdapterTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <functional>

#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/utils.h"
#include "test/testutils/qt/qt_utils.h"

#include "src/qtui/candidate_item_qmodel_adapter.h"
#include "test/core/candidate_item_test_helper.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

class CandidateItemQModelAdapterTest : public ::testing::Test {
 protected:
  CandidateItemQModelAdapterTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~CandidateItemQModelAdapterTest() { }
  virtual void SetUp() {
    root_item.reset(CandidateItemTestHelper::genCandidateItemTree());

    adapter.setCandidates(*root_item);

    auto abstract_qmodel = dynamic_cast<QAbstractItemModel*>(&adapter);
    ASSERT_NE(nullptr, abstract_qmodel);
  }
  // virtual void TearDown() { }

  using ItemCheckerFunc =
      std::function<void(const CandidateItem& item,
                         const QModelIndex& index,
                         int role)>;
  void assertRoleItemData(const CandidateItem& parent_item,
                          const QModelIndex& parent_index, int role,
                          ItemCheckerFunc checker);

  // region: objects test subject depends on
  std::unique_ptr<CandidateItem> root_item;
  // endregion

  // region: test subject
  CandidateItemQModelAdapter adapter;
  // endregion

  // region: object depends on test subject
  // endregion
};

void CandidateItemQModelAdapterTest::assertRoleItemData(
    const CandidateItem& parent_item,
    const QModelIndex& parent_index, int role,
    ItemCheckerFunc checker) {
  for (int i = 0; i < parent_item.num_children(); ++i) {
    auto item = parent_item[i];
    auto expect_text = U8StringToQString(item->text());

    auto index = adapter.index(i, 0, parent_index);

    CUSTOM_ASSERT(checker(*item, index, role));

    CUSTOM_ASSERT(assertRoleItemData(*item, index, role, checker));
  }
}

TEST_F(CandidateItemQModelAdapterTest,
       should_DisplayRole_data_be_item_text) { // NOLINT
  auto checker =
      [this](const CandidateItem& item, const QModelIndex& index, int role) {
    auto expect_text = U8StringToQString(item.text());
    auto cell_data = adapter.data(index, role);
    auto actual_text = cell_data.toString();
    ASSERT_EQ(expect_text, actual_text);
  };

  CUSTOM_ASSERT(assertRoleItemData(*root_item,
                                   QModelIndex(),
                                   Qt::DisplayRole,
                                   checker));
}

TEST_F(CandidateItemQModelAdapterTest,
       should_TooltipRole_data_be_item_description) { // NOLINT
  auto checker =
      [this](const CandidateItem& item, const QModelIndex& index, int role) {
    auto expect_desc = U8StringToQString(item.description());
    auto cell_data = adapter.data(index, role);
    auto actual_desc = cell_data.toString();
    ASSERT_EQ(expect_desc, actual_desc);
  };

  CUSTOM_ASSERT(assertRoleItemData(*root_item,
                                   QModelIndex(),
                                   Qt::ToolTipRole,
                                   checker));
}

TEST_F(CandidateItemQModelAdapterTest,
       should_unselectable_if_item_is_unselectable) { // NOLINT
  // Setup fixture
  auto checker =
      [this](const CandidateItem& item, const QModelIndex& index, int role) {
    V_UNUSED(role);
    auto expect_selectable = item.isSelectable();

    auto index_flags = adapter.flags(index);
    bool actual_selectable = !!(index_flags & Qt::ItemIsSelectable);
    ASSERT_EQ(expect_selectable, actual_selectable);
  };

  // Exercise system
  for (int i = 0; i < root_item->num_children(); ++i) {
    auto item = (*root_item)[i];
    item->setUnselectable();
  }

  // Verify results
  CUSTOM_ASSERT(assertRoleItemData(*root_item,
                                   QModelIndex(),
                                   0,  // not used
                                   checker));
}

TEST_F(CandidateItemQModelAdapterTest,
       should_index_internalPointer_be_item_pointer) { // NOLINT
  // Setup fixture
  auto checker =
      [this](const CandidateItem& item, const QModelIndex& index, int role) {
    V_UNUSED(role);
    auto expect_ptr = &item;
    auto actual_ptr = index.internalPointer();

    ASSERT_EQ(expect_ptr, actual_ptr);
  };

  // Verify results
  CUSTOM_ASSERT(assertRoleItemData(*root_item,
                                   QModelIndex(),
                                   0,
                                   checker));
}

TEST_F(CandidateItemQModelAdapterTest,
       should_keep_parent_child_relationship) { // NOLINT
  // Setup fixture
  auto checker =
      [this](const CandidateItem& item, const QModelIndex& index, int role) {
    V_UNUSED(role);
    auto parent_item = item.parent();
    auto parent_index = adapter.parent(index);
    auto parent_index_item = parent_index.internalPointer();

    ASSERT_EQ(parent_item, parent_index_item);
  };

  // Verify results
  CUSTOM_ASSERT(assertRoleItemData(*root_item,
                                   QModelIndex(),
                                   0,
                                   checker));
}
