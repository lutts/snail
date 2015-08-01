//-*- TestCaseName: AttributeLayoutTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <algorithm>

#include <QApplication>
#include <QLabel>
#include <QToolButton>
#include <QAction>

#include "test/testutils/gmock_common.h"
#include "test/testutils/qt/gui_tester.h"


#include "src/utils/utils.h"
#include "test/qtui/test_attr_display_block_generator.h"
#include "src/qtui/attribute_layout.h"
#include "utils/mock_command.h"
#include "qtui/i_attribute_view.h"

using namespace utils;  // NOLINT
using namespace utils::tests;  // NOLINT
using namespace snailcore;  // NOLINT

static int dummy_argc { 0 };
static QApplication app(dummy_argc, nullptr);

class AttributeLayoutTest : public ::testing::Test
                          , public GuiTester {
 protected:
  AttributeLayoutTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeLayoutTest() { }
  virtual void SetUp() {
  }
  // virtual void TearDown() { }

  void layoutAttributes(const ExpectationHolder& expectation,
                        bool debug = false);

  void assertAttributeLabelEqual(
      const ExpectationHolder& expectation, int row, int column);
  void assertAttributeViewWidgetEqual(
      const ExpectationHolder& expectation, int row, int column);
  void assertCommandCanBeTriggered(
      const ExpectationHolder& expectation, int row, int column);
  void assertCellEmpty(int row, int column);

  void verifyLayoutResult(const ExpectationHolder& expectation);

  // region: test subject
  AttributeLayout attr_layout;
  // endregion
};

void AttributeLayoutTest::layoutAttributes(
    const ExpectationHolder& expectation,  bool debug) {
  int num_attrs = expectation.num_attrs();

  attr_layout.beginLayout(num_attrs);
  for (int index = 0; index < num_attrs; ++index) {
    if (expectation.isGroupAt(index)) {
      auto group_block = expectation.attrGroupBlockAt(index);
      if (debug) {
        std::cout << "group@" << group_block << " " << group_block->label
                  << " @ index " << index << std::endl;
      }
      auto priv_data = attr_layout.layoutAttributeGroupDisplayBlock(*group_block);
      group_block->view_priv_data = priv_data;
    } else {
      auto attr_block = expectation.attrViewBlockAt(index);
      if (debug) {
        std::cout << "attribute@" << attr_block << " " << attr_block->label
                  << " @index " << index
                  << ", widget@" << attr_block->attr_view->getWidget()
                  << std::endl;
      }
      auto priv_data = attr_layout.layoutAttributeDisplayBlock(*attr_block);
      attr_block->view_priv_data = priv_data;
    }
  }
  attr_layout.endLayout();
  QTest::qWait(10);  // TODO(lutts): 10 is enought?
}

void AttributeLayoutTest::assertAttributeLabelEqual(
    const ExpectationHolder& expectation, int row, int column) {
  auto expect_label = expectation.labelAt(row, column);

  QLayoutItem* item = attr_layout.itemAtPosition(row, column);
  ASSERT_NE(nullptr, item) << "label item @row" << row
                           << ",column" << column
                           << " should be " << expect_label;
  QLabel* label = qobject_cast<QLabel*>(item->widget());
  ASSERT_NE(nullptr, label) << "label @row" << row
                            << ", column" << column
                            << " should not be null";
  auto actual_label = QStringToU8String(label->text());
  ASSERT_EQ(expect_label, actual_label)
      << "label @row" << row << ", column" << column
      << " should be " << expect_label << ", but actually is " << actual_label;
}

void AttributeLayoutTest::assertAttributeViewWidgetEqual(
    const ExpectationHolder& expectation, int row, int column) {
  auto expect_widget = expectation.widgetAt(row, column);

  QLayoutItem* item = attr_layout.itemAtPosition(row, column);
  ASSERT_NE(nullptr, item) << "widget item @row" << row
                           << ", column" << column
                           << " should be " << expect_widget;
  ASSERT_EQ(expect_widget, item->widget())
      << "widget @row" << row << ", column" << column
      << " should be " << expect_widget
      << ", but actually is " << item->widget();
}

void AttributeLayoutTest::assertCommandCanBeTriggered(
    const ExpectationHolder& expectation, int row, int column) {
  auto expect_command = expectation.commandAt(row, column);

  QLayoutItem* item = attr_layout.itemAtPosition(row, column);
  ASSERT_NE(nullptr, item) << "command item @row" << row
                           << ", column" << column
                           << " should not be null";

  QWidget* widget = item->widget();
  ASSERT_NE(nullptr, widget) << "Command front widget @row" << row
                             << ", column" << column
                             << " should not be null";

  EXPECT_CALL(*expect_command, redo());

  QTest::mouseClick(widget, Qt::LeftButton);
}

void AttributeLayoutTest::assertCellEmpty(int row, int column) {
  QLayoutItem* item = attr_layout.itemAtPosition(row, column);
  ASSERT_EQ(nullptr, item);
}

void AttributeLayoutTest::verifyLayoutResult(
    const ExpectationHolder& expectation) {
  int row_count = expectation.rowCount();

  int item_count = 0;

  for (int row = 0; row < row_count; ++row) {
    for (int column = 0; column < AttributeLayout::kTotalColumn; ++column) {
      if (column == AttributeLayout::kSeperatorColumn) {
        continue;
      }

      if (expectation.isEmpty(row, column)) {
        CUSTOM_ASSERT(assertCellEmpty(row, column));
        continue;
      }

      ++item_count;

      switch (column) {
        case AttributeLayout::kLeftLabelColumn:
        case AttributeLayout::kRightLabelColumn:
          CUSTOM_ASSERT(assertAttributeLabelEqual(expectation, row, column));
          break;

        case AttributeLayout::kLeftAttrViewColumn:
          // case AttributeLayout::kLeftAddCommandColumn:
        case AttributeLayout::kRightAttrViewColumn:
          //  case AttributeLayout::kRightAddCommandColumn:
          if (expectation.isGroupAt(row, column)) {
            CUSTOM_ASSERT(
                assertCommandCanBeTriggered(expectation, row, column));
          } else {
            CUSTOM_ASSERT(
                assertAttributeViewWidgetEqual(expectation, row, column));
          }
          break;

        case AttributeLayout::kSeperatorColumn:
        default:
          break;
      }  // switch column
    }  // for column
  }    // for row

  ASSERT_EQ(item_count, attr_layout.count());
}

TEST_F(AttributeLayoutTest,
       should_basically_divide_from_the_middle_if_num_attrs_is_even_number) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  expect.setExpectAA(0, attr_pool.createAttr(), attr_pool.createAttr());
  expect.setExpectAA(1, attr_pool.createAttr(), attr_pool.createAttr());
  expect.setExpectAA(2, attr_pool.createAttr(), attr_pool.createAttr());

  // Exercise system
  layoutAttributes(expect);

  // Verify results
  verifyLayoutResult(expect);
}

TEST_F(AttributeLayoutTest,
       should_allow_left_side_plus_one_if_num_attrs_is_odd) { // NOLINT
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  expect.setExpectAA(0, attr_pool.createAttr(), attr_pool.createAttr());
  expect.setExpectAA(1, attr_pool.createAttr(), attr_pool.createAttr());
  expect.setExpectAA(2, attr_pool.createAttr(), attr_pool.createAttr());
  expect.setExpectAA(3, attr_pool.createAttr(), nullptr);

  // Exercise system
  layoutAttributes(expect);

  // Verify results
  verifyLayoutResult(expect);
}

/*
 * group with one or two sub attributes test scenario
 *
 * num_attrs % 2 == 0 && sub_attrs == 1
 * before adjust
 *   xxxxx               sub attr
 *   xxxxx              xxxxx
 *   group              xxxxx
 *
 * after adjust
 *   xxxxx              xxxxx
 *   xxxxx              xxxxx
 *   group
 *    sub attr
 * -----------------------------------
 * num_attrs % 2 == 0 && sub_attrs > 1
 * befor adjust
 *   xxxxx              sub attr
 *   xxxxx              sub attr
 *   xxxxx              xxxxx
 *   group              xxxxx
 *
 * after adjust
 *   xxxxx              group
 *   xxxxx                sub attr
 *   xxxxx                sub attr
 *                      xxxxx
 *                      xxxxx
 *
 * -----------------------------------
 * num_attrs % 2 != 0
 * before ajust
 *   xxxxx               sub attr
 *   xxxxx              xxxxx or sub attr
 *   xxxxx              xxxxx
 *   group
 *
 * after ajust
 *   xxxxx               group
 *   xxxxx                 sub attr
 *   xxxxx               xxxxx or sub attr
 *                       xxxxx
 *
 */

TEST_F(AttributeLayoutTest,
       should_move_sub_attr_to_left_when_cut_at_first_sub_attr_of_a_group_which_has_only_one_sub_attr_on_even_num_attrs) { // NOLINT
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  auto group_block = attr_pool.createGroup();
  auto sub_attr_block = attr_pool.createAttr(group_block);

  expect.setExpectAA(0, attr_pool.createAttr(),  attr_pool.createAttr());
  expect.setExpectAA(1, attr_pool.createAttr(),  attr_pool.createAttr());
  expect.setExpectGA(2, group_block,             nullptr);
  expect.setExpectAA(3, sub_attr_block,          nullptr);

  // Exercise system
  layoutAttributes(expect);

  // Verify result
  verifyLayoutResult(expect);
}

TEST_F(AttributeLayoutTest,
       should_move_group_to_right_when_cut_at_first_sub_attr_of_a_group_which_has_more_than_one_sub_attr_on_even_num_attrs) { // NOLINT
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  auto group_block = attr_pool.createGroup();
  auto sub_attr_block1 = attr_pool.createAttr(group_block);
  auto sub_attr_block2 = attr_pool.createAttr(group_block);

  expect.setExpectAG(0, attr_pool.createAttr(), group_block);
  expect.setExpectAA(1, attr_pool.createAttr(), sub_attr_block1);
  expect.setExpectAA(2, attr_pool.createAttr(), sub_attr_block2);
  expect.setExpectAA(3, nullptr,                 attr_pool.createAttr());
  expect.setExpectAA(4, nullptr,                 attr_pool.createAttr());

  // Exercise system
  layoutAttributes(expect);

  // Verify result
  verifyLayoutResult(expect);
}

TEST_F(AttributeLayoutTest,
       should_move_group_to_right_when_cut_at_first_sub_attr_of_a_group_which_has_only_one_sub_attr_on_odd_num_attr) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  auto group_block = attr_pool.createGroup();
  auto sub_attr_block = attr_pool.createAttr(group_block);

  expect.setExpectAG(0, attr_pool.createAttr(), group_block);
  expect.setExpectAA(1, attr_pool.createAttr(), sub_attr_block);
  expect.setExpectAA(2, attr_pool.createAttr(), attr_pool.createAttr());
  expect.setExpectAA(3, nullptr,                 attr_pool.createAttr());

  // Exercise system
  layoutAttributes(expect);

  // Verify results
  verifyLayoutResult(expect);
}

TEST_F(AttributeLayoutTest,
       should_move_group_to_right_when_cut_at_first_sub_attr_of_a_group_which_has_more_than_one_sub_attr_on_odd_num_attr) { // NOLINT
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  auto group_block = attr_pool.createGroup();
  auto sub_attr_block1 = attr_pool.createAttr(group_block);
  auto sub_attr_block2 = attr_pool.createAttr(group_block);

  expect.setExpectAG(0, attr_pool.createAttr(), group_block);
  expect.setExpectAA(1, attr_pool.createAttr(), sub_attr_block1);
  expect.setExpectAA(2, attr_pool.createAttr(), sub_attr_block2);
  expect.setExpectAA(3, nullptr,                 attr_pool.createAttr());

  // Exercise system
  layoutAttributes(expect);

  // Verify results
  verifyLayoutResult(expect);
}

TEST_F(AttributeLayoutTest,
       should_treat_group_as_normal_attrs_if_group_has_no_sub_attrs_on_even_num_attr) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  // Layout:
  // xxxxx           xxxxx
  // group           xxxxx
  expect.setExpectAA(0, attr_pool.createAttr(), attr_pool.createAttr());
  expect.setExpectGA(1, attr_pool.createGroup(), attr_pool.createAttr());

  // Exercise system
  layoutAttributes(expect);

  // Verify results
  verifyLayoutResult(expect);
}

TEST_F(AttributeLayoutTest,
       should_treat_group_as_normal_attrs_if_group_has_no_sub_attrs_on_odd_num_attr) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  // Layout:
  // xxxxx           xxxxx
  // xxxxx           xxxxx
  // group
  expect.setExpectAA(0, attr_pool.createAttr(), attr_pool.createAttr());
  expect.setExpectAA(1, attr_pool.createAttr(), attr_pool.createAttr());
  expect.setExpectGA(2, attr_pool.createGroup(), nullptr);

  // Exercise system
  layoutAttributes(expect);

  // Verify results
  verifyLayoutResult(expect);
}

TEST_F(AttributeLayoutTest,
       should_not_split_if_there_is_only_one_group_and_the_group_has_less_than_3_subviews_and_there_is_no_other_toplevel_attributes) { // NOLINT
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  auto group_block = attr_pool.createGroup();
  auto sub_attr1 = attr_pool.createAttr(group_block);
  auto sub_attr2 = attr_pool.createAttr(group_block);

  // expect layout:
  //    group                 (empty)
  //      sub_attr            (empty)
  //      sub_attr            (empty)
  expect.setExpectGA(0, group_block, nullptr);
  expect.setExpectAA(1, sub_attr1,   nullptr);
  expect.setExpectAA(2, sub_attr2,   nullptr);

  // Exercise system
  layoutAttributes(expect);

  // Verify results
  verifyLayoutResult(expect);
}

TEST_F(AttributeLayoutTest,
       should_do_normal_split_if_there_is_only_one_group_which_has_GE_than_3_subviews_and_there_is_no_other_toplevel_attributes) { // NOLINT
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  auto group_block = attr_pool.createGroup();
  auto sub_attr1 = attr_pool.createAttr(group_block);
  auto sub_attr2 = attr_pool.createAttr(group_block);
  auto sub_attr3 = attr_pool.createAttr(group_block);

  // expect layout:
  //    group                 sub_attr2
  //      sub_attr1           sub_attr3
  expect.setExpectGA(0, group_block, sub_attr2);
  expect.setExpectAA(1, sub_attr1,   sub_attr3);

  // Exercise system
  layoutAttributes(expect);

  // Verify results
  verifyLayoutResult(expect);
}

TEST_F(AttributeLayoutTest,
       should_not_create_button_for_null_commands) { // NOLINT
  TestAttributePool attr_pool;

  auto attr_block = attr_pool.createAttr();

  auto group_block = attr_pool.createGroup();
  group_block->add_command = nullptr;

  ExpectationHolder expect;
  expect.setExpectGA(0, group_block, attr_block);

  // Exercise system
  layoutAttributes(expect);

  // Verify results
  verifyLayoutResult(expect);
}

TEST_F(AttributeLayoutTest,
       should_only_show_the_first_label_and_hide_other_equal_labels_if_there_are_successive_equal_labels) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;

  auto left_attr0 = attr_pool.createAttr("Attribute 1");
  auto left_attr1 = attr_pool.createAttr("Attribute 1");
  auto left_attr2 = attr_pool.createAttr("Attribute 2");  // same as begin of group

  auto left_group0 = attr_pool.createGroup("Attribute 2");  // same name as prev and next
  auto left_sub_attr00 = attr_pool.createAttr(left_group0, "Attribute 2");
  auto left_sub_attr01 = attr_pool.createAttr(left_group0, "Attribute 2");
  auto left_sub_attr02 = attr_pool.createAttr(left_group0, "Attribute 3");
  auto left_sub_attr03 = attr_pool.createAttr(left_group0, "Attribute 3");

  auto left_group1 = attr_pool.createGroup("Attribute 3");  // same name as prev and next
  auto left_sub_attr10 = attr_pool.createAttr(left_group1, "Attribute 3");
  auto left_sub_attr11 = attr_pool.createAttr(left_group1, "Attribute 3");
  auto left_sub_attr12 = attr_pool.createAttr(left_group1, "Attribute 4");
  auto left_sub_attr13 = attr_pool.createAttr(left_group1, "Attribute 4");

  auto left_attr3 = attr_pool.createAttr("Attribute 4");  // same as end of group
  auto left_attr4 = attr_pool.createAttr("Attribute 5");
  auto left_attr5 = attr_pool.createAttr("Attribute 6");

  ExpectationHolder expect;

  expect.setExpectAA(0, left_attr0, attr_pool.createAttr());
  expect.setExpectAA(1, left_attr1, attr_pool.createAttr());
  expect.setExpectAA(2, left_attr2, attr_pool.createAttr());

  expect.setExpectGA(3, left_group0, attr_pool.createAttr());
  expect.setExpectAA(4, left_sub_attr00, attr_pool.createAttr());
  expect.setExpectAA(5, left_sub_attr01, attr_pool.createAttr());
  expect.setExpectAA(6, left_sub_attr02, attr_pool.createAttr());
  expect.setExpectAA(7, left_sub_attr03, attr_pool.createAttr());

  expect.setExpectGA(8, left_group1, attr_pool.createAttr());
  expect.setExpectAA(9, left_sub_attr10, attr_pool.createAttr());
  expect.setExpectAA(10, left_sub_attr11, attr_pool.createAttr());
  expect.setExpectAA(11, left_sub_attr12, attr_pool.createAttr());
  expect.setExpectAA(12, left_sub_attr13, attr_pool.createAttr());

  expect.setExpectAA(13, left_attr3, attr_pool.createAttr());
  expect.setExpectAA(14, left_attr4, attr_pool.createAttr());
  expect.setExpectAA(15, left_attr5, attr_pool.createAttr());

  // label expected
  expect.setLeftLabelAt(0, "Attribute 1");
  expect.setLeftLabelAt(1, "Attribute 1");
  expect.setLeftLabelAt(2, "Attribute 2");

  expect.setLeftLabelAt(3, "Attribute 2");  // actually is a group
  expect.setLeftLabelAt(4, "Attribute 2");
  expect.setLeftLabelAt(5, "");  // should hide
  expect.setLeftLabelAt(6, "Attribute 3");
  expect.setLeftLabelAt(7, "");            // should hide

  expect.setLeftLabelAt(8, "Attribute 3");  // actually is a group
  expect.setLeftLabelAt(9, "Attribute 3");
  expect.setLeftLabelAt(10, "");  // should hide
  expect.setLeftLabelAt(11, "Attribute 4");
  expect.setLeftLabelAt(12, "");  // should hide

  expect.setLeftLabelAt(13, "Attribute 4");
  expect.setLeftLabelAt(14, "Attribute 5");
  expect.setLeftLabelAt(15, "Attribute 6");

  // Exercise system
  layoutAttributes(expect);

  // Verify results
  verifyLayoutResult(expect);
}

TEST_F(AttributeLayoutTest,
       should_be_able_to_relayout_with_different_set_of_attrs_and_groups) { // NOLINT
  // Setup fixture
  TestAttributePool attr_pool;
  ExpectationHolder expect;

  auto group = attr_pool.createGroup();
  auto sub_attr1 = attr_pool.createAttr(group);

  expect.setExpectAA(0, attr_pool.createAttr(), attr_pool.createAttr());
  expect.setExpectGA(1, group, attr_pool.createAttr());
  expect.setExpectAA(2, sub_attr1, nullptr);

  layoutAttributes(expect, true);
  verifyLayoutResult(expect);

  // relayout with different set of attrs and groups
  auto sub_attr2 = attr_pool.createAttr(group);
  ExpectationHolder another_expect;

  another_expect.setExpectAG(0, attr_pool.createAttr(), group);
  another_expect.setExpectAA(1, attr_pool.createAttr(), sub_attr1);
  another_expect.setExpectAA(2, attr_pool.createAttr(), sub_attr2);
  another_expect.setExpectAA(3, nullptr, attr_pool.createAttr());

  layoutAttributes(another_expect, true);
  verifyLayoutResult(another_expect);
}

// test Add, Delete, Update
