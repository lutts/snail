//-*- TestCaseName: AttributeLayoutTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QApplication>
#include <QLabel>
#include <QToolButton>
#include <QAction>

#include "test/testutils/gmock_common.h"
#include "test/testutils/qt/gui_tester.h"

#include "src/utils/utils.h"
#include "src/qtui/attribute_layout.h"
#include "utils/mock_command.h"
#include "qtui/mock_attribute_view.h"
#include "snail/i_attribute_display_block.h"

using namespace utils;  // NOLINT
using namespace utils::tests;  // NOLINT
using namespace snailcore;  // NOLINT

static int dummy_argc { 0 };
static QApplication app(dummy_argc, nullptr);

class TestAttributeGenerator {
 public:
  virtual ~TestAttributeGenerator() = default;

  virtual int num_attrs() const = 0;
  virtual int left_side_count() const = 0;

  virtual AttributeViewDisplayBlock attrViewBlockAt(int index) const = 0;
  virtual U8String labelAt(int index) const = 0;
  virtual const QWidget* widgetAt(int index) const = 0;
  virtual MockCommand* eraseCommandAt(int index) const = 0;
  virtual MockCommand* editCommandAt(int index) const = 0;

  virtual bool isGroup(int index) const {
    (void)index;
    return false;
  }

  virtual AttributeGroupDisplayBlock* attrGroupBlockAt(int index) const {
    (void)index;
    return nullptr;
  }

  virtual MockCommand* addCommandAt(int index) const {
    (void)index;
    return nullptr;
  }
};

using DummyWidget = QLabel;

class AttributeLayoutTest : public TestWithParam<int>
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

  void layoutAttributes(const TestAttributeGenerator& attr_generator);

  void assertAttributeLabelEqual(
      const utils::U8String& expect_label, int row, int column);
  void assertAttributeViewWidgetEqual(
      const QWidget* expect_widget, int row, int column);
  void assertCommandCanBeTriggered(
      MockCommand* command, int row, int column);
  void assertCellEmpty(int row, int column);

  void verifyAttributeViewDisplayBlock(
      const TestAttributeGenerator& attr_generator,
      int block_index, int row, int start_column);
  void verifyAttributeGroupDisplayBlock(
      const TestAttributeGenerator& attr_generator,
      int block_index, int row, int start_column);
  void verifyLayoutResult(const TestAttributeGenerator& attr_generator);

  // region: test subject
  AttributeLayout attr_layout;
  // endregion
};

INSTANTIATE_TEST_CASE_P(
    VariousNumberOfAttributes,
    AttributeLayoutTest,
    ::testing::Range(1, 10));

void AttributeLayoutTest::layoutAttributes(
    const TestAttributeGenerator& attr_generator) {
  int num_attrs = attr_generator.num_attrs();

  attr_layout.beginAddAttributeDisplayBlock(num_attrs);
  for (int i = 0; i < num_attrs; ++i) {
    if (attr_generator.isGroup(i)) {
      // std::cout << "group @ index " << i << std::endl;
      attr_layout.addAttributeGroupDisplayBlock(
          attr_generator.attrGroupBlockAt(i));
    } else {
      // std::cout << attr_generator.labelAt(i) << " @index " << i << std::endl;
      attr_layout.addAttributeDisplayBlock(attr_generator.attrViewBlockAt(i));
    }
  }
  attr_layout.endAddAttributeDisplayBlock();
}

void AttributeLayoutTest::assertAttributeLabelEqual(
    const utils::U8String& expect_label, int row, int column) {
  QLayoutItem* item = attr_layout.itemAtPosition(row, column);
  if (!expect_label.empty()) {
    ASSERT_NE(nullptr, item) << "label item @row" << row
                             << ",column" << column
                             << " should not be null, "
                             << "label = " << expect_label;
  } else {
    ASSERT_EQ(nullptr, item) << "empty label @row" << row
                             << ",column" << column
                             << " should got null cell item";
    return;
  }
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
    const QWidget* expect_widget, int row, int column) {
  QLayoutItem* item = attr_layout.itemAtPosition(row, column);
  ASSERT_NE(nullptr, item) << "widget item @row" << row
                           << ", column" << column
                           << " should not be null";
  ASSERT_EQ(expect_widget, item->widget())
      << "widget @row" << row << ", column" << column
      << " should be " << expect_widget
      << ", but actually is " << item->widget();
}

void AttributeLayoutTest::assertCommandCanBeTriggered(
    MockCommand* command, int row, int column) {
  QLayoutItem* item = attr_layout.itemAtPosition(row, column);
  if (command) {
    ASSERT_NE(nullptr, item) << "command item @row" << row
                             << ", column" << column
                             << " should not be null";
  } else {
    ASSERT_EQ(nullptr, item) << "command item @row" << row
                             << ", column" << column
                             << " should be empty because command is null";
    return;
  }
  QWidget* widget = item->widget();
  ASSERT_NE(nullptr, widget) << "Command front widget @row" << row
                             << ", column" << column
                             << " should not be null";

  EXPECT_CALL(*command, redo());

  QTest::mouseClick(widget, Qt::LeftButton);
}

void AttributeLayoutTest::assertCellEmpty(int row, int column) {
  QLayoutItem* item = attr_layout.itemAtPosition(row, column);
  ASSERT_EQ(nullptr, item);
}

void AttributeLayoutTest::verifyAttributeGroupDisplayBlock(
    const TestAttributeGenerator& attr_generator,
    int block_index, int row, int start_column) {
  int label_column = start_column + 0;
  int add_cmd_column = start_column + 1;

  auto expect_label = attr_generator.labelAt(block_index);
  auto expect_add_cmd = attr_generator.addCommandAt(block_index);

  CUSTOM_ASSERT(assertAttributeLabelEqual(expect_label, row, label_column));
  CUSTOM_ASSERT(assertCommandCanBeTriggered(expect_add_cmd,
                                            row, add_cmd_column));
}

void AttributeLayoutTest::verifyAttributeViewDisplayBlock(
    const TestAttributeGenerator& attr_generator,
    int block_index, int row, int start_column) {
  int label_column = start_column + 0;
  int attr_view_column = start_column + 1;
  int erase_cmd_column = start_column + 2;
  int edit_cmd_column = start_column + 3;

  auto expect_label = attr_generator.labelAt(block_index);
  auto expect_widget = attr_generator.widgetAt(block_index);
  auto erase_cmd = attr_generator.eraseCommandAt(block_index);
  auto edit_cmd = attr_generator.editCommandAt(block_index);

  CUSTOM_ASSERT(assertAttributeLabelEqual(expect_label, row, label_column));
  CUSTOM_ASSERT(assertAttributeViewWidgetEqual(expect_widget,
                                               row, attr_view_column));
  CUSTOM_ASSERT(assertCommandCanBeTriggered(erase_cmd,
                                            row, erase_cmd_column));
  CUSTOM_ASSERT(assertCommandCanBeTriggered(edit_cmd,
                                            row, edit_cmd_column));
}

void AttributeLayoutTest::verifyLayoutResult(
    const TestAttributeGenerator& attr_generator) {
  int num_attrs = attr_generator.num_attrs();
  int left_side_count = attr_generator.left_side_count();
  int right_side_count = num_attrs - left_side_count;

  for (int block_index = 0; block_index < num_attrs;  ++block_index) {
    // std::cout << "check block index " << block_index << std::endl;
    int row = block_index;
    int start_column = 0;
    if (block_index >= left_side_count) {
      row -= left_side_count;
      start_column = AttributeLayout::kRightSideFirstColumn;
    }

    if (attr_generator.isGroup(block_index)) {
      // std::cout << "verify group at row " << row << std::endl;
      CUSTOM_ASSERT(verifyAttributeGroupDisplayBlock(attr_generator,
                                                     block_index,
                                                     row, start_column));
    } else {
      // std::cout << "verify attr at row " << row << std::endl;
      CUSTOM_ASSERT(verifyAttributeViewDisplayBlock(attr_generator,
                                                    block_index,
                                                    row, start_column));
    }
  }

  int large_row_count = 0;
  int small_row_count = 0;
  int column_offset = 0;

  if (right_side_count < left_side_count) {
    large_row_count = left_side_count;
    small_row_count = right_side_count;
    // empty is on the right
    column_offset = AttributeLayout::kRightSideFirstColumn;
  } else if (right_side_count > left_side_count) {
    large_row_count = right_side_count;
    small_row_count = left_side_count;
    // empty is on the left
    column_offset = 0;
  }

  if (large_row_count != small_row_count) {
    const int empty_cell_start_row = small_row_count;
    const int empty_cell_end_row = large_row_count;
    const int empty_cell_start_column = column_offset;
    const int empty_cell_end_column = column_offset +
                                      AttributeLayout::kNumColumnPerSide;

    for (int row = empty_cell_start_row;
         row < empty_cell_end_row; ++row) {
      for (int column = empty_cell_start_column;
           column < empty_cell_end_column;
           ++column) {
        CUSTOM_ASSERT(assertCellEmpty(row, column));
      }
    }
  }
}

class BasicAttributesGenerator : public TestAttributeGenerator {
 public:
  explicit BasicAttributesGenerator(int num_attrs)
      : num_attrs_(num_attrs)
      , attr_views(num_attrs)
      , widgets(num_attrs)
      , erase_commands(num_attrs)
      , edit_commands(num_attrs) {
    for (int i = 0; i < num_attrs_; ++i) {
      ON_CALL(attr_views[i], getWidget())
          .WillByDefault(Return(&widgets[i]));
      ON_CALL(erase_commands[i], display_text())
          .WillByDefault(
              Return(utils::formatString("erase command No.{1}", i)));
      ON_CALL(edit_commands[i], display_text())
          .WillByDefault(
              Return(utils::formatString("edit command No.{1}", i)));

      attr_view_blocks.push_back(
          { utils::formatString("basic attribute No.{1}", i),
                &attr_views[i],
                &erase_commands[i],
                &edit_commands[i],
                false});
    }
  }
  virtual ~BasicAttributesGenerator() = default;

  int num_attrs() const override { return num_attrs_; }

  int left_side_count() const override {
    int count = num_attrs_ / 2;
    if (num_attrs_ % 2)
      ++count;

    return count;
  }

  AttributeViewDisplayBlock attrViewBlockAt(int index) const override {
    return attr_view_blocks[index];
  }

  U8String labelAt(int index) const override {
    return attr_view_blocks[index].label;
  }

  const QWidget* widgetAt(int index) const override {
    return attr_view_blocks[index].attr_view->getWidget();
  }

  MockCommand* eraseCommandAt(int index) const override {
    return const_cast<MockCommand*>(&erase_commands[index]);
  }

  MockCommand* editCommandAt(int index) const override {
    return const_cast<MockCommand*>(&edit_commands[index]);
  }

 private:
  int num_attrs_;
  std::vector<MockAttributeView> attr_views;
  std::vector<DummyWidget> widgets;
  std::vector<MockCommand> erase_commands;
  std::vector<MockCommand> edit_commands;

  std::vector<AttributeViewDisplayBlock> attr_view_blocks;

 private:
  SNAIL_DISABLE_COPY(BasicAttributesGenerator)
};

TEST_P(AttributeLayoutTest,
       should_basically_divide_from_the_middle_but_allow_left_side_plus_one_if_num_attrs_is_odd) { // NOLINT
  // Setup fixture
  int num_attrs = GetParam();
  BasicAttributesGenerator attr_generator(num_attrs);

  int expect_left_side_count = num_attrs / 2;
  if (num_attrs % 2) {
    ++expect_left_side_count;
  }


  // Exercise system
  layoutAttributes(attr_generator);

  // Verify results
  ASSERT_EQ(expect_left_side_count, attr_generator.left_side_count());
  verifyLayoutResult(attr_generator);
}

#include "utils/basic_utils.h"

class OnlyOneGroupAttributeGenerator : public TestAttributeGenerator {
 public:
  explicit OnlyOneGroupAttributeGenerator(int num_sub_attrs)
      : num_sub_attrs_(num_sub_attrs)
      , sub_attr_views(num_sub_attrs)
      , sub_widgets(num_sub_attrs)
      , sub_erase_commands(num_sub_attrs)
      , sub_edit_commands(num_sub_attrs) {
    group.label = utils::U8String{"The Group"};
    group.add_command = &add_command;
    group.sub_attr_count = num_sub_attrs;

    for (int i = 0; i < num_sub_attrs; ++i) {
      ON_CALL(sub_attr_views[i], getWidget())
          .WillByDefault(Return(&sub_widgets[i]));
      ON_CALL(sub_erase_commands[i], display_text())
          .WillByDefault(
              Return(utils::formatString("sub erase command No.{1}", i)));
      ON_CALL(sub_edit_commands[i], display_text())
          .WillByDefault(
              Return(utils::formatString("sub edit command No.{1}", i)));

      sub_attr_view_blocks.push_back(
          { utils::formatString("sub attribute No.{1}", i),
                &sub_attr_views[i],
                &sub_erase_commands[i],
                &sub_edit_commands[i],
                true});
    }
  }

  virtual ~OnlyOneGroupAttributeGenerator() = default;

  int num_attrs() const override {
    return num_sub_attrs_ + 1;
  }

  int left_side_count() const {
    if (num_sub_attrs_ <= 2) {
      return num_attrs();
    } else {
      int count = num_attrs() / 2;
      if (num_attrs() % 2)
        ++count;

      return count;
    }
  }

  bool isGroup(int index) const override {
    return index == 0;
  }

  AttributeGroupDisplayBlock* attrGroupBlockAt(int index) const override {
    if (index == 0)
      return const_cast<AttributeGroupDisplayBlock*>(&group);
    else
      return nullptr;
  }

  MockCommand* addCommandAt(int index) const override {
    [index]() {
      ASSERT_EQ(0, index)
          << "OnlyGroupGenerator index 0 is group, not " << index;
    }();

    return const_cast<MockCommand*>(&add_command);
  }

  AttributeViewDisplayBlock attrViewBlockAt(int index) const override {
    [index]() {
      ASSERT_NE(0, index) << "OnlyGroupGenerator index 0 is group, not attr";
    }();

    return sub_attr_view_blocks[index - 1];
  }

  U8String labelAt(int index) const override {
    if (index == 0)
      return group.label;
    else
      return sub_attr_view_blocks[index - 1].label;
  }

  const QWidget* widgetAt(int index) const override {
    [index]() {
      ASSERT_NE(0, index) << "OnlyGroupGenerator index 0 is group, no widget";
    }();

    return &sub_widgets[index - 1];
  }

  MockCommand* eraseCommandAt(int index) const override {
    [index]() {
      ASSERT_NE(0, index)
          << "OnlyGroupGenerator index 0 is group, no erase cmd";
    }();

    return const_cast<MockCommand*>(&sub_erase_commands[index - 1]);
  }

  MockCommand* editCommandAt(int index) const override {
    [index]() {
      ASSERT_NE(0, index)
          << "OnlyGroupGenerator index 0 is group, no edit cmd";
    }();

    return const_cast<MockCommand*>(&sub_edit_commands[index - 1]);
  }

 private:
  int num_sub_attrs_;

  AttributeGroupDisplayBlock group;
  MockCommand add_command;

  std::vector<MockAttributeView> sub_attr_views;
  std::vector<DummyWidget> sub_widgets;
  std::vector<MockCommand> sub_erase_commands;
  std::vector<MockCommand> sub_edit_commands;

  std::vector<AttributeViewDisplayBlock> sub_attr_view_blocks;

 private:
  SNAIL_DISABLE_COPY(OnlyOneGroupAttributeGenerator)
};


/*
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

class CutAtFirstSubAttrOfGroupAttributeGenerator
    : public TestAttributeGenerator {
 public:
  CutAtFirstSubAttrOfGroupAttributeGenerator(int total_num_attrs,
                                             int num_sub_attrs)
      : num_attrs_(total_num_attrs)
      , num_sub_attrs_(num_sub_attrs)
      , basic_attrs(total_num_attrs - 1 - num_sub_attrs)
      , group_attrs(num_sub_attrs) { }

  int num_attrs() const override {
    return num_attrs_;
  }

  int left_side_count() const override {
    // based on group_index
    int count = group_index() + 1;

    if (should_move_subattr_to_left())
      ++count;
    else if (should_move_group_to_right())
      --count;

    return count;
  }

  bool isGroup(int index) const override {
    return index == group_index();
  }

  AttributeGroupDisplayBlock* attrGroupBlockAt(int index) const override {
    if (is_group_attrs_index(index)) {
      int grp_attr_index = to_group_attrs_index(index);
      return group_attrs.attrGroupBlockAt(grp_attr_index);
    }

    return nullptr;
  }

  AttributeViewDisplayBlock attrViewBlockAt(int index) const override {
    if (is_group_attrs_index(index)) {
      int grp_attr_index = to_group_attrs_index(index);
      return group_attrs.attrViewBlockAt(grp_attr_index);
    } else {
      int basic_attr_index = to_basic_attrs_index(index);
      return basic_attrs.attrViewBlockAt(basic_attr_index);
    }
  }

  U8String labelAt(int index) const override {
    if (is_group_attrs_index(index)) {
      int grp_attr_index = to_group_attrs_index(index);
      return group_attrs.labelAt(grp_attr_index);
    } else {
      int basic_attr_index = to_basic_attrs_index(index);
      return basic_attrs.labelAt(basic_attr_index);
    }
  }

  const QWidget* widgetAt(int index) const {
    if (is_group_attrs_index(index)) {
      int grp_attr_index = to_group_attrs_index(index);
      return group_attrs.widgetAt(grp_attr_index);
    } else {
      int basic_attr_index = to_basic_attrs_index(index);
      return basic_attrs.widgetAt(basic_attr_index);
    }
  }

  MockCommand* eraseCommandAt(int index) const override {
    if (is_group_attrs_index(index)) {
      int grp_attr_index = to_group_attrs_index(index);
      return group_attrs.eraseCommandAt(grp_attr_index);
    } else {
      int basic_attr_index = to_basic_attrs_index(index);
      return basic_attrs.eraseCommandAt(basic_attr_index);
    }
  }

  MockCommand* editCommandAt(int index) const override {
    if (is_group_attrs_index(index)) {
      int grp_attr_index = to_group_attrs_index(index);
      return group_attrs.editCommandAt(grp_attr_index);
    } else {
      int basic_attr_index = to_basic_attrs_index(index);
      return basic_attrs.editCommandAt(basic_attr_index);
    }
  }


  MockCommand* addCommandAt(int index) const override {
    if (is_group_attrs_index(index)) {
      int grp_attr_index = to_group_attrs_index(index);
      return group_attrs.addCommandAt(grp_attr_index);
    }

    return nullptr;
  }

 private:
  bool should_move_group_to_right() const {
    if (num_attrs_ % 2) {
      // in this case, left side in therory plus one,
      // if we move the subattr to left, then left side
      // will plus three, to avoid this, we move the group
      // to the right side, which makes right_side_count = left_side_count + 1
      return true;
    } else {
      return (num_sub_attrs_ > 1);
    }
  }

  bool should_move_subattr_to_left() const {
    if ( (num_attrs_ % 2 == 0) && (num_sub_attrs_ == 1) )
      return true;
    else
      return false;
  }

  int group_index() const {
    // put group at the last row of unadjusted left side
    int unadjusted_left_side_count = num_attrs_ / 2;
    if (num_attrs_ % 2)
      ++unadjusted_left_side_count;

    return --unadjusted_left_side_count;
  }

  int min_group_attr_index() const {
    return group_index();
  }

  int max_group_attr_index() const {
    return group_index() + num_sub_attrs_;
  }

  bool is_group_attrs_index(int index) const {
    return (index >= min_group_attr_index()) &&
        (index <= max_group_attr_index());
  }

  int to_group_attrs_index(int index) const {
    return index - group_index();
  }

  int to_basic_attrs_index(int index) const {
    if (index > max_group_attr_index())
      return index - group_attrs.num_attrs();

    return index;
  }

  int num_attrs_;
  int num_sub_attrs_;

  BasicAttributesGenerator basic_attrs;
  OnlyOneGroupAttributeGenerator group_attrs;
};

TEST_P(AttributeLayoutTest,
       should_treat_the_single_sub_attr_and_group_as_a_whole_if_the_group_has_only_one_sub_attr) { // NOLINT
  // Setup fixture
  int num_attrs = GetParam();

  // min requirement: 1 normal attr + 1 group + 1 sub attr
  const int MIN_REQUIREMENT_NUM_ATTR = 1 + 1 + 1;
  if (num_attrs < MIN_REQUIREMENT_NUM_ATTR) {
    SUCCEED();
    return;
  }

  CutAtFirstSubAttrOfGroupAttributeGenerator attr_generator(num_attrs, 1);

  int right_side_count = num_attrs - attr_generator.left_side_count();
  if (num_attrs % 2 == 0) {
    int diff = attr_generator.left_side_count() - right_side_count;
    ASSERT_EQ(2, diff);
  } else {
    int diff = right_side_count - attr_generator.left_side_count();
    ASSERT_EQ(1, diff);
  }

  // Exercise system
  layoutAttributes(attr_generator);

  // Verify results
  verifyLayoutResult(attr_generator);
}

TEST_P(AttributeLayoutTest,
       should_move_group_to_right_side_if_split_on_first_subview_of_a_group_which_has_more_that_one_subview) { // NOLINT
  // Setup fixture
  int num_attrs = GetParam();

  // min requirement: 1 normal attr + 1 group + 2 sub attr
  const int MIN_REQUIREMENT_NUM_ATTR = 1 + 1 + 2;
  if (num_attrs < MIN_REQUIREMENT_NUM_ATTR) {
    SUCCEED();
    return;
  }

  CutAtFirstSubAttrOfGroupAttributeGenerator attr_generator(num_attrs, 2);

  int right_side_count = num_attrs - attr_generator.left_side_count();
  int diff = right_side_count - attr_generator.left_side_count();
  if (num_attrs % 2 == 0) {
    ASSERT_EQ(2, diff);
  } else {
    ASSERT_EQ(1, diff);
  }

  // Exercise system
  layoutAttributes(attr_generator);

  // Verify results
  verifyLayoutResult(attr_generator);
}

TEST_P(AttributeLayoutTest,
       should_not_split_if_there_is_only_one_group_and_the_group_has_less_than_3_subviews_and_there_is_no_other_toplevel_attributes) { // NOLINT
  // Setup fixture
  int num_attrs = GetParam();

  if (num_attrs > 2) {
    SUCCEED();
    return;
  }
  OnlyOneGroupAttributeGenerator attr_generator(num_attrs);

  int right_side_count = attr_generator.num_attrs()
                         - attr_generator.left_side_count();
  ASSERT_EQ(0, right_side_count);

  // Exercise system
  layoutAttributes(attr_generator);

  // Verify results
  verifyLayoutResult(attr_generator);
}

TEST_P(AttributeLayoutTest,
       should_do_normal_split_if_there_is_only_one_group_which_has_GE_tha_3_subviews_and_there_is_no_other_toplevel_attributes) { // NOLINT
  // Setup fixture
  int num_attrs = GetParam();

  if (num_attrs < 3) {
    SUCCEED();
    return;
  }

  OnlyOneGroupAttributeGenerator attr_generator(num_attrs);

  int expect_left_count = attr_generator.num_attrs() / 2;
  if (attr_generator.num_attrs() % 2)
    ++expect_left_count;

  ASSERT_EQ(expect_left_count, attr_generator.left_side_count());

  // Exercise system
  layoutAttributes(attr_generator);

  // Verify results
  verifyLayoutResult(attr_generator);
}

class NullAddCommandGroupAttributeGenerator
    : public OnlyOneGroupAttributeGenerator {
 public:
  NullAddCommandGroupAttributeGenerator()
      :OnlyOneGroupAttributeGenerator(1) { }

  virtual ~NullAddCommandGroupAttributeGenerator() = default;

  AttributeGroupDisplayBlock* attrGroupBlockAt(int index) const override {
    auto grp_block = OnlyOneGroupAttributeGenerator::attrGroupBlockAt(index);
    if (grp_block) {
      grp_block->add_command = nullptr;
    }

    return grp_block;
  }

  MockCommand* addCommandAt(int index) const override {
    (void)index;
    return nullptr;
  }

 private:
  SNAIL_DISABLE_COPY(NullAddCommandGroupAttributeGenerator);
};

TEST_F(AttributeLayoutTest,
       should_not_create_button_for_null_add_commands) { // NOLINT
  // Setup fixture
  NullAddCommandGroupAttributeGenerator attr_generator;

  // Exercise system
  layoutAttributes(attr_generator);

  // Verify results
  verifyLayoutResult(attr_generator);
}

class SparseNullEditCommmandAttributeGenerator
    : public BasicAttributesGenerator {
 public:
  SparseNullEditCommmandAttributeGenerator()
      : BasicAttributesGenerator(5) {
  }
  virtual ~SparseNullEditCommmandAttributeGenerator() = default;

  AttributeViewDisplayBlock attrViewBlockAt(int index) const override {
    auto attr_block = BasicAttributesGenerator::attrViewBlockAt(index);
    if (index % 2)
      attr_block.edit_command = nullptr;

    return attr_block;
  }

  MockCommand* editCommandAt(int index) const override {
    if (index % 2) {
      return nullptr;
    } else {
      return BasicAttributesGenerator::editCommandAt(index);
    }
  }

 private:
  SNAIL_DISABLE_COPY(SparseNullEditCommmandAttributeGenerator)
};


TEST_F(AttributeLayoutTest,
       should_not_create_buttons_for_null_edit_commands) { // NOLINT
  // Setup fixture
  SparseNullEditCommmandAttributeGenerator attr_generator;

  // Exercise system
  layoutAttributes(attr_generator);

  // Verify results
  verifyLayoutResult(attr_generator);
}

class EqualLabelAttributesGenerator : public TestAttributeGenerator {
 public:
  EqualLabelAttributesGenerator()
      : basic_attrs_begin(begin_count)
      , group_attrs_middle1(middle1_count)
      , group_attrs_middle2(middle1_count)
      , basic_attrs_end(end_count)
      , padding_attrs(padding_count) {
    // label to layouter
    // begin
    label_to_layout.emplace_back("Attribute 1");
    label_to_layout.emplace_back("Attribute 1");
    label_to_layout.emplace_back("Attribute 2");  // same as begin of group
    // middle 1
    label_to_layout.emplace_back("Attribute 2");  // acutally is a group
    label_to_layout.emplace_back("Attribute 2");
    label_to_layout.emplace_back("Attribute 2");
    label_to_layout.emplace_back("Attribute 3");
    label_to_layout.emplace_back("Attribute 3");
    // middle 2
    label_to_layout.emplace_back("Attribute 3");  // acutally is a group
    label_to_layout.emplace_back("Attribute 3");
    label_to_layout.emplace_back("Attribute 3");
    label_to_layout.emplace_back("Attribute 4");
    label_to_layout.emplace_back("Attribute 4");
    // end
    label_to_layout.emplace_back("Attribute 4");  // same as end of group
    label_to_layout.emplace_back("Attribute 5");
    label_to_layout.emplace_back("Attribute 6");

    // label expected
    // begin
    label_expected.emplace_back("Attribute 1");
    label_expected.emplace_back("Attribute 1");
    label_expected.emplace_back("Attribute 2");
    // middle 1
    label_expected.emplace_back("Attribute 2");  // actually is a group
    label_expected.emplace_back("Attribute 2");
    label_expected.emplace_back("");
    label_expected.emplace_back("Attribute 3");
    label_expected.emplace_back("");
    // middle 2
    label_expected.emplace_back("Attribute 3");  // actually is a group
    label_expected.emplace_back("Attribute 3");
    label_expected.emplace_back("");
    label_expected.emplace_back("Attribute 4");
    label_expected.emplace_back("");
    // end
    label_expected.emplace_back("Attribute 4");
    label_expected.emplace_back("Attribute 5");
    label_expected.emplace_back("Attribute 6");
  }

  virtual ~EqualLabelAttributesGenerator() = default;

  int num_attrs() const override {
    return left_count + padding_count;
  }

  int left_side_count() const override {
    return left_count;
  }

  AttributeViewDisplayBlock attrViewBlockAt(int index) const override {
    const auto& generator = generatorOfIndex(index);
    auto attr_block = generator.attrViewBlockAt(adjust_index(index));

    if (index <= left_max_index) {
      attr_block.label = label_to_layout[index];
    }

    return attr_block;
  }

  AttributeGroupDisplayBlock* attrGroupBlockAt(int index) const override {
    if (index == group1_index) {
      auto group = group_attrs_middle1.attrGroupBlockAt(0);
      group->label = label_to_layout[group1_index];
      return group;
    } else if (index == group2_index) {
      auto group = group_attrs_middle2.attrGroupBlockAt(0);
      group->label = label_to_layout[group2_index];
      return group;
    } else {
      return nullptr;
    }
  }

  U8String labelAt(int index) const {
    if (index <= left_max_index) {
      return label_expected[index];
    } else {
      return padding_attrs.labelAt(index - left_count);
    }
  }

  // other functions
  const QWidget* widgetAt(int index) const override {
    const auto& generator = generatorOfIndex(index);
    return generator.widgetAt(adjust_index(index));
  }

  MockCommand* eraseCommandAt(int index) const override {
    const auto& generator = generatorOfIndex(index);
    return generator.eraseCommandAt(adjust_index(index));
  }

  MockCommand* editCommandAt(int index) const override {
    const auto& generator = generatorOfIndex(index);
    return generator.editCommandAt(adjust_index(index));
  }

  bool isGroup(int index) const {
    return (index == group1_index) ||
        (index == group2_index);
  }

  MockCommand* addCommandAt(int index) const {
    if (index == group1_index) {
      return group_attrs_middle1.addCommandAt(0);
    } else if (index == group2_index) {
      return group_attrs_middle2.addCommandAt(0);
    } else {
      return nullptr;
    }
  }

  const TestAttributeGenerator& generatorOfIndex(int index) const {
    if (index <= begin_max_index)
      return basic_attrs_begin;
    else if (index <= middle1_max_index)
      return group_attrs_middle1;
    else if (index <= middle2_max_index)
      return group_attrs_middle2;
    else if (index <= left_max_index)
      return basic_attrs_end;
    else
      return padding_attrs;
  }

  int adjust_index(int index) const {
    if (index <= begin_max_index)
      return index;
    else if (index <= middle1_max_index)
      return index - begin_count;
    else if (index <= middle2_max_index)
      return index - (middle1_max_index + 1);
    else if (index <= left_max_index)
      return index - (middle2_max_index + 1);
    else
      return index - left_count;
  }

 private:
  static constexpr int begin_count = 3;
  static constexpr int begin_max_index = begin_count - 1;

  static constexpr int middle1_count = 4;  // sub attributes count
  static constexpr int group1_index = begin_count;
  static constexpr int middle1_max_index = begin_count
                                           + 1 + middle1_count
                                           - 1;

  static constexpr int middle2_count = 4;  // sub attributes count
  static constexpr int group2_index = begin_count + 1 + middle1_count;
  static constexpr int middle2_max_index = begin_count
                                           + 1 + middle1_count
                                           + 1 + middle2_count
                                           - 1;

  static constexpr int end_count = 3;
  static constexpr int left_count = begin_count
                                    + 1 + middle1_count
                                    + 1 + middle2_count
                                    + end_count;
  static constexpr int left_max_index = left_count - 1;

  static constexpr int padding_count = left_count;

  BasicAttributesGenerator basic_attrs_begin;
  OnlyOneGroupAttributeGenerator group_attrs_middle1;
  OnlyOneGroupAttributeGenerator group_attrs_middle2;
  BasicAttributesGenerator basic_attrs_end;
  BasicAttributesGenerator padding_attrs;

  std::vector<utils::U8String> label_to_layout;
  std::vector<utils::U8String> label_expected;

 private:
  SNAIL_DISABLE_COPY(EqualLabelAttributesGenerator)
};


TEST_F(AttributeLayoutTest,
       should_only_show_the_first_label_and_hide_other_equal_labels_if_there_are_successive_equal_labels) { // NOLINT
  // Setup fixture
  EqualLabelAttributesGenerator attr_generator;

  // Exercise system
  layoutAttributes(attr_generator);

  // Verify results
  verifyLayoutResult(attr_generator);
}

TEST_F(AttributeLayoutTest,
       should_clear_layout_items_when_beginAddAttributeDisplayBlock_called) { // NOLINT
  // Setup fixture
  BasicAttributesGenerator attr_generator(10);
  layoutAttributes(attr_generator);
  verifyLayoutResult(attr_generator);

  // Exercise system
  attr_layout.beginAddAttributeDisplayBlock(std::rand());

  // Verify results
  ASSERT_EQ(0, attr_layout.count());

  // can be re-layout with other attributes
  BasicAttributesGenerator another_generator(7);
  layoutAttributes(another_generator);
  verifyLayoutResult(another_generator);
}
