//-*- TestCaseName: CandidateItemTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/utils.h"

#include "test/core/candidate_item_test_helper.h"

namespace snailcore {
namespace tests {

class CandidateItemTest : public ::testing::Test {
 protected:
  CandidateItemTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~CandidateItemTest() { }
  // virtual void SetUp() { }
  // virtual void TearDown() { }
};

TEST_F(CandidateItemTest,
       should_default_constructor_build_and_root_item) { // NOLINT
  // Setup fixture

  // Expectations

  // Exercise system
  auto item = utils::make_unique<CandidateItem>();

  // Verify results
  ASSERT_TRUE(item->isRoot());
}

TEST_F(CandidateItemTest,
       should_be_able_to_construct_with_text_and_description) { // NOLINT
  // Setup fixture
  auto expect_text = xtestutils::genRandomString();
  auto expect_desc = xtestutils::genRandomString();

  // Exercise system
  auto item = utils::make_unique<CandidateItem>(expect_text, expect_desc);
  auto actual_text = item->text();
  auto actual_desc = item->description();

  // Verify results
  ASSERT_EQ(expect_text, actual_text);
  ASSERT_EQ(expect_desc, actual_desc);
}

TEST_F(CandidateItemTest,
       should_be_able_to_change_to_unselectable) { // NOLINT
  // Setup fixture
  auto item = utils::make_unique<CandidateItem>(
      xtestutils::genRandomString(),
      xtestutils::genRandomString());

  ASSERT_TRUE(item->isSelectable());

  // Exercise system
  item->setUnselectable();

  // Verify results
  ASSERT_FALSE(item->isSelectable());
}

TEST_F(CandidateItemTest,
       should_be_able_to_build_item_hierarchy) { // NOLINT
  // Setup fixture

  // Expectations

  // Exercise system
  auto root_item = utils::make_unique<CandidateItem>(
      xtestutils::genRandomString(),
      xtestutils::genRandomString());
  auto child_item1 = utils::make_unique<CandidateItem>(
      xtestutils::genRandomString(),
      xtestutils::genRandomString(), root_item.get());

  auto child_item2 = utils::make_unique<CandidateItem>(
      xtestutils::genRandomString(),
      xtestutils::genRandomString(), root_item.get());

  auto subchild_item1 = utils::make_unique<CandidateItem>(
      xtestutils::genRandomString(),
      xtestutils::genRandomString(), child_item1.get());

  auto subchild_item2 = utils::make_unique<CandidateItem>(
      xtestutils::genRandomString(),
      xtestutils::genRandomString(), child_item2.get());

  // Verify results
  ASSERT_EQ(2, root_item->num_children());

  auto actual_child_item1 = (*root_item)[0];
  ASSERT_EQ(child_item1.get(), actual_child_item1);
  ASSERT_EQ(root_item.get(), child_item1->parent());

  auto actual_child_item2 = (*root_item)[1];
  ASSERT_EQ(child_item2.get(), actual_child_item2);
  ASSERT_EQ(root_item.get(), child_item2->parent());

  ASSERT_EQ(1, actual_child_item1->num_children());
  ASSERT_EQ(subchild_item1.get(), (*actual_child_item1)[0]);
  ASSERT_EQ(actual_child_item1, subchild_item1->parent());

  ASSERT_EQ(1, actual_child_item2->num_children());
  ASSERT_EQ(subchild_item2.get(), (*actual_child_item2)[0]);
  ASSERT_EQ(actual_child_item2, subchild_item2->parent());
}

TEST_F(CandidateItemTest,
       should_remove_from_parent_child_list_when_child_destroy) { // NOLINT
  // Setup fixture
  auto root_item = utils::make_unique<CandidateItem>(
      xtestutils::genRandomString(),
      xtestutils::genRandomString());
  auto child_item1 = utils::make_unique<CandidateItem>(
      xtestutils::genRandomString(),
      xtestutils::genRandomString(), root_item.get());

  auto child_item2 = utils::make_unique<CandidateItem>(
      xtestutils::genRandomString(),
      xtestutils::genRandomString(), root_item.get());

  ASSERT_EQ(2, root_item->num_children());

  auto actual_child_item1 = (*root_item)[0];
  auto actual_child_item2 = (*root_item)[1];
  ASSERT_EQ(child_item1.get(), actual_child_item1);
  ASSERT_EQ(child_item2.get(), actual_child_item2);

  // Expectations

  // Exercise system
  child_item1.reset();

  // Verify results
  ASSERT_EQ(1, root_item->num_children());
  ASSERT_EQ(child_item2.get(), (*root_item)[0]);
}

class MockCandidateItem : public CandidateItem {
 public:
  MockCandidateItem(
      utils::U8String text,
      utils::U8String description,
      CandidateItem* parent = nullptr)
      : CandidateItem(text, description, parent) { }

  ~MockCandidateItem() { destruct(); }

  MOCK_METHOD0(destruct, void());
};

TEST_F(CandidateItemTest,
       should_destroy_all_childs_when_destroy_root) { // NOLINT
  // Setup fixture
  auto root_item = utils::make_unique<MockCandidateItem>(
      xtestutils::genRandomString(),
      xtestutils::genRandomString());
  auto child_item1 = new MockCandidateItem(
      xtestutils::genRandomString(),
      xtestutils::genRandomString(), root_item.get());

  auto child_item2 = new MockCandidateItem(
      xtestutils::genRandomString(),
      xtestutils::genRandomString(), root_item.get());

  ASSERT_EQ(2, root_item->num_children());

  auto actual_child_item1 = (*root_item)[0];
  auto actual_child_item2 = (*root_item)[1];
  ASSERT_EQ(child_item1, actual_child_item1);
  ASSERT_EQ(child_item2, actual_child_item2);

  // Expectations
  EXPECT_CALL(*child_item1, destruct());
  EXPECT_CALL(*child_item2, destruct());
  EXPECT_CALL(*root_item, destruct());

  // Exercise system
  root_item.reset();
}

TEST_F(CandidateItemTest,
       should_be_able_to_generate_flat_item_list) { // NOLINT
  // Exercise system
  const int expect_num_items = 3;
  std::unique_ptr<CandidateItem> root_item(
      CandidateItemTestHelper::genCandidateItemTree(
          1,  // num_level
          expect_num_items));

  // Verify results
  auto actual_num_items = root_item->num_children();
  ASSERT_EQ(expect_num_items, actual_num_items);

  for (int i = 0; i < actual_num_items; ++i) {
    ASSERT_EQ(0, (*root_item)[i]->num_children());
  }

  // Teardown fixture
}

TEST_F(CandidateItemTest,
       should_be_able_to_generate_item_tree) { // NOLINT
  // Setup fixture
  const int expect_num_level = 3;
  const int expect_num_items = 3;

  std::unique_ptr<CandidateItem> root_item(
      CandidateItemTestHelper::genCandidateItemTree(
          expect_num_level, expect_num_items));

  // Verify results
  ASSERT_EQ(expect_num_items, root_item->num_children());
  // level1 (3 items)
  for (int i = 0; i < expect_num_items; ++i) {  // level (3 items)
    auto item_L1 = (*root_item)[i];
    ASSERT_EQ(expect_num_items, item_L1->num_children());
    for (int j = 0; j < expect_num_items; ++j) {  // level2 (3 * 3 items)
      auto item_L2 = (*item_L1)[j];
      ASSERT_EQ(expect_num_items, item_L2->num_children());

      for (int k = 0; k < expect_num_items; ++k) {  // level3 (3 * 3 * 3 items)
        auto item_L3 = (*item_L2)[k];
        ASSERT_EQ(0, item_L3->num_children());
      }
    }
  }
}


}  // namespace tests
}  // namespace snailcore
