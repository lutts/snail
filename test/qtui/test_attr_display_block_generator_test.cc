//-*- TestCaseName: TestAttrDisplayBlockGeneratorTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <map>

#include <QApplication>

#include "test/testutils/gmock_common.h"
#include "test/qtui/test_attr_display_block_generator.h"
#include "src/qtui/attribute_layout.h"
#include "qtui/mock_attribute_view.h"
#include "qtui/mock_work_attribute_view.h"
#include "snail/mock_attribute_display_block.h"
#include "utils/mock_command.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

bool operator==(const AttributeViewDisplayBlock& a,
                const AttributeViewDisplayBlock& b) {
  return (a.label == b.label) &&
      (a.attr_view == b.attr_view) &&
      (a.is_in_group == b.is_in_group) &&
      (a.view_priv_data == b.view_priv_data);
}

static int dummy_argc { 0 };
static QApplication app(dummy_argc, nullptr);

class TestAttrDisplayBlockGeneratorTest : public ::testing::Test {
 protected:
  TestAttrDisplayBlockGeneratorTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~TestAttrDisplayBlockGeneratorTest() { }
  virtual void SetUp() { }
  // virtual void TearDown() { }
};

enum Sides {
  kLeftSide = 0,
  kRightSide = 1,
  kSideCount,
};

void assertPositionIsAttr(
    const ExpectationHolder& expect_holder,
    AttributeViewDisplayBlock* test_attrs[][kSideCount],
    int row, int side) {
  int label_column = AttributeLayout::kLeftLabelColumn;
  int widget_column = AttributeLayout::kLeftAttrViewColumn;

  if (side == kRightSide) {
    label_column = AttributeLayout::kRightLabelColumn;
    widget_column = AttributeLayout::kRightAttrViewColumn;
  }

  ASSERT_FALSE(expect_holder.isGroupAt(row, widget_column));

  auto attr_block = test_attrs[row][side];

  auto expect_label = attr_block->label;
  auto actual_label =
      expect_holder.labelAt(row, label_column);
  ASSERT_NE(utils::U8String{""}, expect_label);
  ASSERT_EQ(expect_label, actual_label);

  auto expect_widget = attr_block->attr_view->getWidget();
  auto actual_widget =
      expect_holder.widgetAt(row, widget_column);
  ASSERT_NE(nullptr, expect_widget);
  ASSERT_EQ(expect_widget, actual_widget);
}

void assertPositionIsGroup(
    const ExpectationHolder& expect_holder,
    AttributeGroupDisplayBlock* test_groups[][kSideCount],
    int row, int side) {
  int label_column = AttributeLayout::kLeftLabelColumn;
  int add_cmd_column = AttributeLayout::kLeftAddCommandColumn;

  if (side == kRightSide) {
    label_column = AttributeLayout::kRightLabelColumn;
    add_cmd_column = AttributeLayout::kRightAddCommandColumn;
  }

  ASSERT_TRUE(expect_holder.isGroupAt(row, add_cmd_column));

  auto group_block = test_groups[row][side];

  auto expect_label = group_block->label;
  auto actual_label =
      expect_holder.labelAt(row, label_column);
  ASSERT_NE(utils::U8String{""}, expect_label);
  ASSERT_EQ(expect_label, actual_label);

  auto expect_add_cmd = group_block->add_command;
  auto actual_add_cmd =
      expect_holder.addCommandAt(row, add_cmd_column);
  ASSERT_NE(nullptr, expect_add_cmd);
  ASSERT_EQ(expect_add_cmd, actual_add_cmd);
}

void assertPositionEmpty(const ExpectationHolder& expect_holder,
                         int row, int side) {
  int label_column = AttributeLayout::kLeftLabelColumn;
  int widget_column = AttributeLayout::kLeftAttrViewColumn;

  if (side == kRightSide) {
    label_column = AttributeLayout::kRightLabelColumn;
    widget_column = AttributeLayout::kRightAttrViewColumn;
  }

  ASSERT_TRUE(
      expect_holder.isEmpty(row, label_column));
  ASSERT_TRUE(
      expect_holder.isEmpty(row, widget_column));
}

void assertAttributeAtIndex(const ExpectationHolder& expect_holder,
                            AttributeViewDisplayBlock* test_attrs[][kSideCount],
                            int index, int row, int side) {
  ASSERT_EQ(test_attrs[row][side], expect_holder.attrViewBlockAt(index));
  ASSERT_FALSE(expect_holder.isGroupAt(index));
}

void assertGroupAtIndex(const ExpectationHolder& expect_holder,
                        AttributeGroupDisplayBlock* test_groups[][kSideCount],
                        int index, int row, int side) {
  ASSERT_EQ(test_groups[row][side], expect_holder.attrGroupBlockAt(index));
  ASSERT_TRUE(expect_holder.isGroupAt(index));
}

void assertAttrValid(AttributeViewDisplayBlock* attr) {
  ASSERT_NE(nullptr, attr);
  ASSERT_FALSE(attr->label.empty());
  ASSERT_NE(nullptr, attr->attr_view);
  ASSERT_NE(nullptr, attr->attr_view->getWidget());
  ASSERT_EQ(nullptr, attr->view_priv_data);
}

void assertAttrNotEqual(AttributeViewDisplayBlock* attr_a,
                        AttributeViewDisplayBlock* attr_b) {

#define ASSERT_FIELD(field)                     \
  ASSERT_NE(attr_a->field, attr_b->field);

  ASSERT_FIELD(label);
  ASSERT_FIELD(attr_view);
  ASSERT_FIELD(attr_view->getWidget());

#undef ASSERT_FIELD
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       should_be_able_to_create_attr) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;

  // Exercise system
  auto attr0 = attr_pool.createAttr();
  auto attr1 = attr_pool.createAttr();

  // Verify results
  CUSTOM_ASSERT(assertAttrValid(attr0));
  ASSERT_FALSE(attr0->is_in_group);

  CUSTOM_ASSERT(assertAttrValid(attr1));
  ASSERT_FALSE(attr0->is_in_group);

  CUSTOM_ASSERT(assertAttrNotEqual(attr0, attr1));
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       should_be_able_to_create_attr_with_label) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  auto expect_label = xtestutils::genRandomString();

  // Exercise system
  auto attr = attr_pool.createAttr(expect_label);

  // Verify results
  CUSTOM_ASSERT(assertAttrValid(attr));
  ASSERT_EQ(expect_label, attr->label);
}

void assertGroupValid(AttributeGroupDisplayBlock* group) {
  ASSERT_NE(nullptr, group);
  ASSERT_FALSE(group->label.empty());
  ASSERT_NE(nullptr, group->add_command);
  ASSERT_FALSE(group->add_command->display_text().empty());
  ASSERT_EQ(0, group->sub_attr_count);
  ASSERT_EQ(nullptr, group->view_priv_data);
}

void assertGroupNotEqual(AttributeGroupDisplayBlock* group_a,
                         AttributeGroupDisplayBlock* group_b) {
#define ASSERT_FIELD(field)                     \
  ASSERT_NE(group_a->field, group_b->field);

  ASSERT_FIELD(label);
  ASSERT_FIELD(add_command);
  ASSERT_FIELD(add_command->display_text());

#undef ASSERT_FIELD
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       should_be_able_to_create_group) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;

  // Exercise system
  auto group0 = attr_pool.createGroup();
  auto group1 = attr_pool.createGroup();

  // Verify results
  CUSTOM_ASSERT(assertGroupValid(group0));
  CUSTOM_ASSERT(assertGroupValid(group1));
  CUSTOM_ASSERT(assertGroupNotEqual(group0, group1));
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       should_be_able_to_create_group_with_label) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  auto expect_label = xtestutils::genRandomString();

  // Exercise system
  auto group = attr_pool.createGroup(expect_label);

  // Verify results
  CUSTOM_ASSERT(assertGroupValid(group));
  ASSERT_EQ(expect_label, group->label);
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       should_be_able_to_create_attr_with_group) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  auto group = attr_pool.createGroup();

  ASSERT_EQ(0, group->sub_attr_count);

  // Exercise system
  auto attr = attr_pool.createAttr(group);

  // Verify results
  CUSTOM_ASSERT(assertAttrValid(attr));
  ASSERT_TRUE(attr->is_in_group);
  ASSERT_EQ(1, group->sub_attr_count);
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       should_be_able_to_build_expectations) { // NOLINT
  int expect_row_count = 5;
  TestAttributePool attr_pool;

  AttributeViewDisplayBlock* test_attrs[expect_row_count][kSideCount];
  for (int row = 0; row < expect_row_count; ++row) {
    for (int col = 0; col < kSideCount; ++col) {
      test_attrs[row][col] = attr_pool.createAttr();
    }
  }

  AttributeGroupDisplayBlock* test_groups[expect_row_count][kSideCount];
  for (int row = 0; row < expect_row_count; ++row) {
    for (int col = 0; col < kSideCount; ++col) {
      test_groups[row][col] = attr_pool.createGroup();
    }
  }

  ExpectationHolder expect_holder;
  ASSERT_EQ(0, expect_holder.num_attrs());
  ASSERT_EQ(0, expect_holder.rowCount());

  expect_holder.setExpectAA(0, test_attrs[0][kLeftSide], test_attrs[0][kRightSide]);
  expect_holder.setExpectAG(1, test_attrs[1][kLeftSide], test_groups[1][kRightSide]);
  expect_holder.setExpectGA(2, test_groups[2][kLeftSide], test_attrs[2][kRightSide]);
  expect_holder.setExpectGG(3, test_groups[3][kLeftSide], test_groups[3][kRightSide]);
  expect_holder.setExpectAA(4, test_attrs[4][kLeftSide], nullptr);

  // check layout interface
  int expect_num_attrs = 2 * expect_row_count - 1 /* nullptr */;
  ASSERT_EQ(expect_num_attrs, expect_holder.num_attrs());
  ASSERT_EQ(expect_row_count, expect_holder.rowCount());

  // check index@
  CUSTOM_ASSERT(assertAttributeAtIndex(expect_holder, test_attrs, 0, 0, kLeftSide));
  CUSTOM_ASSERT(assertAttributeAtIndex(expect_holder, test_attrs, 1, 1, kLeftSide));
  CUSTOM_ASSERT(assertGroupAtIndex(expect_holder, test_groups,    2, 2, kLeftSide));
  CUSTOM_ASSERT(assertGroupAtIndex(expect_holder, test_groups,    3, 3, kLeftSide));
  CUSTOM_ASSERT(assertAttributeAtIndex(expect_holder, test_attrs, 4, 4, kLeftSide));

  CUSTOM_ASSERT(assertAttributeAtIndex(expect_holder, test_attrs, 5, 0, kRightSide));
  CUSTOM_ASSERT(assertGroupAtIndex(expect_holder, test_groups,    6, 1, kRightSide));
  CUSTOM_ASSERT(assertAttributeAtIndex(expect_holder, test_attrs, 7, 2, kRightSide));
  CUSTOM_ASSERT(assertGroupAtIndex(expect_holder, test_groups,    8, 3, kRightSide));

  // check expectation interface
  // check left side
  CUSTOM_ASSERT(assertPositionIsAttr(expect_holder, test_attrs, 0, kLeftSide));
  CUSTOM_ASSERT(assertPositionIsAttr(expect_holder, test_attrs, 1, kLeftSide));
  CUSTOM_ASSERT(assertPositionIsGroup(expect_holder, test_groups, 2, kLeftSide));
  CUSTOM_ASSERT(assertPositionIsGroup(expect_holder, test_groups, 3, kLeftSide));
  CUSTOM_ASSERT(assertPositionIsAttr(expect_holder, test_attrs, 4, kLeftSide));

  // check right side
  CUSTOM_ASSERT(assertPositionIsAttr(expect_holder, test_attrs, 0, kRightSide));
  CUSTOM_ASSERT(assertPositionIsGroup(expect_holder, test_groups, 1, kRightSide));
  CUSTOM_ASSERT(assertPositionIsAttr(expect_holder, test_attrs, 2, kRightSide));
  CUSTOM_ASSERT(assertPositionIsGroup(expect_holder, test_groups, 3, kRightSide));
  CUSTOM_ASSERT(assertPositionEmpty(expect_holder, 4, kRightSide));
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       test_associateAttributeToGroup) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;

  auto attr_block0 = attr_pool.createAttr();
  auto attr_block1 = attr_pool.createAttr();
  auto attr_block2 = attr_pool.createAttr();
  auto attr_block3 = attr_pool.createAttr();

  auto group_block0 = attr_pool.createGroup();
  auto group_block1 = attr_pool.createGroup();
  auto group_block2 = attr_pool.createGroup();

  // Exercise system
  attr_pool.associateAttributeToGroup(attr_block0,
                                      group_block0);
  attr_pool.associateAttributeToGroup(attr_block1,
                                      group_block0);

  attr_pool.associateAttributeToGroup(attr_block2,
                                      group_block1);

  // Verify results
  ASSERT_TRUE(attr_block0->is_in_group);
  ASSERT_TRUE(attr_block1->is_in_group);
  ASSERT_TRUE(attr_block2->is_in_group);
  ASSERT_FALSE(attr_block3->is_in_group);

  ASSERT_EQ(2, group_block0->sub_attr_count);
  ASSERT_EQ(1, group_block1->sub_attr_count);
  ASSERT_EQ(0, group_block2->sub_attr_count);
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       should_be_able_set_expected_label_at_row_with_side) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  auto left_attr = attr_pool.createAttr();
  auto right_group = attr_pool.createGroup();

  expect.setExpectAG(0, left_attr, right_group);

  auto left_label_before =
      expect.labelAt(0, AttributeLayout::kLeftLabelColumn);
  auto right_label_before =
      expect.labelAt(0, AttributeLayout::kRightLabelColumn);

  ASSERT_EQ(left_attr->label, left_label_before);
  ASSERT_EQ(right_group->label, right_label_before);

  auto new_left_label =
      xtestutils::genRandomDifferentString(left_label_before);
  auto new_right_label =
      xtestutils::genRandomDifferentString(right_label_before);

  // Exercise system
  expect.setLeftLabelAt(0, new_left_label);
  expect.setRightLabelAt(0, new_right_label);

  // Verify results
  auto actual_left_label =
      expect.labelAt(0, AttributeLayout::kLeftLabelColumn);
  auto actual_right_label =
      expect.labelAt(0, AttributeLayout::kRightLabelColumn);

  ASSERT_EQ(new_left_label, actual_left_label);
  ASSERT_EQ(new_right_label, actual_right_label);
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       should_set_an_empty_label_wont_change_isEmpty_result) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  auto left_attr = attr_pool.createAttr();
  auto right_group = attr_pool.createGroup();

  expect.setExpectAG(0, left_attr, right_group);

  // Expectations

  // Exercise system
  utils::U8String empty_label;
  expect.setLeftLabelAt(0, empty_label);

  // Verify results
  ASSERT_FALSE(expect.isEmpty(0, AttributeLayout::kLeftLabelColumn));
  ASSERT_FALSE(expect.isEmpty(0, AttributeLayout::kRightLabelColumn));
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       should_be_able_to_save_view_priv_data_to_attr_in_attr_pool) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect_holder;

  auto attr = attr_pool.createAttr();
  auto group = attr_pool.createGroup();

  expect_holder.setExpectAG(0, attr, group);

  void* expect_attr_priv_data = xtestutils::genDummyPointer<void>();
  void* expect_group_priv_data = xtestutils::genDummyPointer<void>();

  // Exercise system
  auto attr_block = expect_holder.attrViewBlockAt(0);
  attr_block->view_priv_data = expect_attr_priv_data;

  auto group_block = expect_holder.attrGroupBlockAt(1);
  group_block->view_priv_data = expect_group_priv_data;

  // Verify results
  ASSERT_EQ(attr, attr_block);
  auto actual_attr_priv_data = attr->view_priv_data;
  ASSERT_EQ(expect_attr_priv_data, actual_attr_priv_data);

  ASSERT_EQ(group, group_block);
  auto actual_group_priv_data = group->view_priv_data;
  ASSERT_EQ(expect_group_priv_data, actual_group_priv_data);
}

// NOTE: the following tests are DISABLED because they are expected to fail
// you should remove the DISABLED prefix to test it

TEST_F(TestAttrDisplayBlockGeneratorTest,
       DISABLED_test_validation_for_same_attr_should_not_add_twice) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect_holder;

  auto attr = attr_pool.createAttr();

  expect_holder.setExpectAA(0, attr, nullptr);
  expect_holder.setExpectAA(1, attr, nullptr);  // should fail
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       DISABLED_test_validation_for_same_row_add_twice) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect_holder;

  // Exercise system
  expect_holder.setExpectAA(0, attr_pool.createAttr(), nullptr);
  expect_holder.setExpectAA(0, attr_pool.createAttr(), nullptr);
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       DISABLED_test_validation_for_row_0_should_exist_if_there_is_rows) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect_holder;

  expect_holder.setExpectAA(1, attr_pool.createAttr(), nullptr);

  // Exercise system
  try {
    expect_holder.attrViewBlockAt(0);
    FAIL() << "should throw some exception";
  } catch (...) { }
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       DISABLED_test_validation_for_continuous_row) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect_holder;

  expect_holder.setExpectAA(0, attr_pool.createAttr(), nullptr);
  expect_holder.setExpectAA(2, attr_pool.createAttr(), nullptr);

  // Exercise system
  expect_holder.attrViewBlockAt(0);
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       DISABLED_test_validation_for_sub_attr_should_after_group) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect_holder;

  attr_pool.associateAttributeToGroup(attr_pool.createAttr(),
                                      attr_pool.createGroup());
  expect_holder.setExpectAA(0, attr_pool.createAttr(), nullptr);

  // Exercise system
  expect_holder.attrViewBlockAt(0);
}

TEST_F(TestAttrDisplayBlockGeneratorTest,
       DISABLED_test_validation_for_sub_attr_count) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect_holder;

  attr_pool.associateAttributeToGroup(attr_pool.createAttr(),
                                      attr_pool.createGroup());
  attr_pool.associateAttributeToGroup(attr_pool.createAttr(),
                                      attr_pool.createGroup());
  expect_holder.setExpectGG(0, attr_pool.createGroup(), nullptr);
  expect_holder.setExpectAA(1, attr_pool.createAttr(), nullptr);
  expect_holder.setExpectAA(2, attr_pool.createAttr(), nullptr);


  // Exercise system
  expect_holder.attrGroupBlockAt(0);
}
