// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/qtui/test_attr_display_block_generator.h"

#include <memory>
#include <vector>

#include <QLabel>

#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"
#include "src/utils/utils.h"
#include "snail/attribute_display_block.h"
#include "qtui/mock_work_attribute_view.h"
#include "qtui/mock_attribute_view.h"
#include "utils/mock_command.h"

#include "src/qtui/attribute_layout.h"

using namespace utils;  // NOLINT
using namespace utils::tests;  // NOLINT
using namespace snailcore;  // NOLINT

class TestAttributePoolImpl {
 public:
  TestAttributePoolImpl();
  virtual ~TestAttributePoolImpl() = default;

  AttributeViewDisplayBlock* createAttr();
  AttributeViewDisplayBlock* createAttr(const utils::U8String& label);
  AttributeViewDisplayBlock* createAttr(
      AttributeGroupDisplayBlock* owner_group);
  AttributeViewDisplayBlock* createAttr(
      AttributeGroupDisplayBlock* owner_group,
      const utils::U8String& label);
  AttributeGroupDisplayBlock* createGroup();
  AttributeGroupDisplayBlock* createGroup(const utils::U8String& label);

  void associateAttributeToGroup(
      AttributeViewDisplayBlock* attr_block,
      AttributeGroupDisplayBlock* attr_group);

  void verifyMocks();

 private:
  using DummyWidget = QLabel;

  std::vector<std::unique_ptr<MockAttributeView>> attr_views;
  std::vector<std::unique_ptr<DummyWidget>> widgets;
  std::vector<std::unique_ptr<MockCommand>> erase_commands;
  std::vector<std::unique_ptr<MockCommand>> edit_commands;
  std::vector<std::unique_ptr<MockCommand>> add_commands;

  std::vector<std::unique_ptr<AttributeGroupDisplayBlock>> attr_group_blocks;
  std::vector<std::unique_ptr<AttributeViewDisplayBlock>> attr_view_blocks;

  int attr_seq { 0 };
  int group_seq { 0 };

 private:
  SNAIL_DISABLE_COPY(TestAttributePoolImpl)

  friend class TestAttributePool;
};

TestAttributePoolImpl::TestAttributePoolImpl() {
}

AttributeViewDisplayBlock*
TestAttributePoolImpl::createAttr() {
  auto default_label =
      utils::formatString("Attribute {1} in {2}", ++attr_seq, this);
  return createAttr(default_label);
}

AttributeViewDisplayBlock*
TestAttributePoolImpl::createAttr(AttributeGroupDisplayBlock* owner_group) {
  auto attr = createAttr();
  associateAttributeToGroup(attr, owner_group);
  return attr;
}

AttributeViewDisplayBlock*
TestAttributePoolImpl::createAttr(
    AttributeGroupDisplayBlock* owner_group,
    const utils::U8String& label) {
  auto attr = createAttr(label);
  associateAttributeToGroup(attr, owner_group);
  return attr;
}

AttributeViewDisplayBlock*
TestAttributePoolImpl::createAttr(const utils::U8String& label) {
  attr_views.push_back(utils::make_unique<MockAttributeView>());
  widgets.push_back(utils::make_unique<DummyWidget>());
  erase_commands.push_back(utils::make_unique<MockCommand>());
  edit_commands.push_back(utils::make_unique<MockCommand>());

  auto& attr_view = attr_views.back();
  auto& widget = widgets.back();
  auto& erase_command = erase_commands.back();
  auto& edit_command = edit_commands.back();

  ON_CALL(*attr_view, getWidget())
      .WillByDefault(Return(widget.get()));
  ON_CALL(*erase_command, display_text())
      .WillByDefault(Return(xtestutils::genRandomString()));
  ON_CALL(*edit_command, display_text())
      .WillByDefault(Return(xtestutils::genRandomString()));

  auto block = utils::make_unique<AttributeViewDisplayBlock>();
  block->label = label;
  block->attr_view = attr_view.get();
  block->erase_command = erase_command.get();
  block->edit_command = edit_command.get();

  auto block_ptr = block.get();

  attr_view_blocks.push_back(std::move(block));

  return block_ptr;
}

AttributeGroupDisplayBlock*
TestAttributePoolImpl::createGroup() {
  auto default_label =
      utils::formatString("Group {1} in {2}", ++group_seq, this);
  return createGroup(default_label);
}

AttributeGroupDisplayBlock*
TestAttributePoolImpl::createGroup(const utils::U8String& label) {
  add_commands.push_back(utils::make_unique<MockCommand>());

  auto& add_command = add_commands.back();

  ON_CALL(*add_command, display_text())
      .WillByDefault(Return(xtestutils::genRandomString()));

  auto block = utils::make_unique<AttributeGroupDisplayBlock>();
  block->label = label;
  block->add_command = add_command.get();

  auto block_ptr = block.get();

  attr_group_blocks.push_back(std::move(block));

  return block_ptr;
}

void TestAttributePoolImpl::associateAttributeToGroup(
    AttributeViewDisplayBlock* attr_block,
    AttributeGroupDisplayBlock* attr_group) {
  attr_block->is_in_group = true;
  ++ attr_group->sub_attr_count;
}

void TestAttributePoolImpl::verifyMocks() {
  for (auto & cmd : erase_commands) {
    Mock::VerifyAndClearExpectations(&cmd);
  }

  for (auto & cmd : edit_commands) {
    Mock::VerifyAndClearExpectations(&cmd);
  }

  for (auto & cmd : add_commands) {
    Mock::VerifyAndClearExpectations(&cmd);
  }
}

TestAttributePool::TestAttributePool()
    : impl(utils::make_unique<TestAttributePoolImpl>()) { }

TestAttributePool::~TestAttributePool() = default;

AttributeViewDisplayBlock*
TestAttributePool::createAttr() {
  return impl->createAttr();
}

AttributeViewDisplayBlock*
TestAttributePool::createAttr(const utils::U8String& label) {
  return impl->createAttr(label);
}

AttributeViewDisplayBlock* TestAttributePool::createAttr(
    AttributeGroupDisplayBlock* owner_group) {
  return impl->createAttr(owner_group);
}

AttributeViewDisplayBlock* TestAttributePool::createAttr(
    AttributeGroupDisplayBlock* owner_group,
    const utils::U8String& label) {
  return impl->createAttr(owner_group, label);
}

AttributeGroupDisplayBlock*
TestAttributePool::createGroup() {
  return impl->createGroup();
}

AttributeGroupDisplayBlock*
TestAttributePool::createGroup(const utils::U8String& label) {
  return impl->createGroup(label);
}

void TestAttributePool::associateAttributeToGroup(
    AttributeViewDisplayBlock* attr_block,
    AttributeGroupDisplayBlock* attr_group) {
  impl->associateAttributeToGroup(attr_block, attr_group);
}

void TestAttributePool::verifyMocks() {
  return impl->verifyMocks();
}

// begin of methods for test cases
void ExpectationHolder::setExpectAA(int row,
                                    AttributeViewDisplayBlock* left_side_attr,
                                    AttributeViewDisplayBlock* right_side_attr) {
  checkRowExist(row);
  checkAttrBlockExist(left_side_attr);
  checkAttrBlockExist(right_side_attr);

  setLeftExpectation(row, left_side_attr);
  setRightExpectation(row, right_side_attr);
}

void ExpectationHolder::setExpectGA(int row,
                                    AttributeGroupDisplayBlock* left_group,
                                    AttributeViewDisplayBlock* right_side_attr) {
  checkRowExist(row);
  checkGroupBlockExist(left_group);
  checkAttrBlockExist(right_side_attr);

  setLeftExpectation(row, left_group);
  setRightExpectation(row, right_side_attr);
}

void ExpectationHolder::setExpectAG(int row,
                                    AttributeViewDisplayBlock* left_side_attr,
                                    AttributeGroupDisplayBlock* right_group) {
  checkRowExist(row);
  checkAttrBlockExist(left_side_attr);
  checkGroupBlockExist(right_group);

  setLeftExpectation(row, left_side_attr);
  setRightExpectation(row, right_group);
}

void ExpectationHolder::setExpectGG(int row,
                                    AttributeGroupDisplayBlock* left_group,
                                    AttributeGroupDisplayBlock* right_group) {
  checkRowExist(row);
  checkGroupBlockExist(left_group);
  checkGroupBlockExist(right_group);

  setLeftExpectation(row, left_group);
  setRightExpectation(row, right_group);
}
// end of methods for test cases

// begin of methods for layout
int ExpectationHolder::num_attrs() const {
  return left_row_data.size() + right_row_data.size();
}

AttributeViewDisplayBlock* ExpectationHolder::attrViewBlockAt(int index) const {
  auto row_data = indexToRowData(index);
  return row_data.attr_block;
}

bool ExpectationHolder::isGroupAt(int index) const {
  auto row_data = indexToRowData(index);
  return row_data.group_block != nullptr;
}

AttributeGroupDisplayBlock* ExpectationHolder::attrGroupBlockAt(int index) const {
  auto row_data = indexToRowData(index);
  return row_data.group_block;
}
// end of method for layout

// begin of methods for verify result
int ExpectationHolder::rowCount() const {
  int left_row_count = left_row_data.size();
  int right_row_count = right_row_data.size();
  return std::max(left_row_count, right_row_count);
}

bool ExpectationHolder::isEmpty(int row, int column) const {
  bool empty = true;

  try {
    empty = position_empty.at(row).at(column);
  } catch (...) { }

  return empty;
}

void ExpectationHolder::setEmpty(int row, int column) {
  position_empty[row][column] = true;
}

U8String ExpectationHolder::labelAt(int row, int column) const {
  return position_to_Label.at(row).at(column);
}

#if 0
U8String ExpectationHolder::labelAt(int index) const {
  int row = index;
  int column = AttributeLayout::kLeftLabelColumn;

  if (index >= static_cast<int>(left_row_data.size())) {
    // right side
    row -=  left_row_data.size();
    column = AttributeLayout::kRightLabelColumn;
  }

  return labelAt(row, column);
}
#endif

void ExpectationHolder::setLeftLabelAt(int row, const utils::U8String& new_label) {
  position_to_Label[row][AttributeLayout::kLeftLabelColumn] = new_label;
  checkLabelEmpty(row, AttributeLayout::kLeftLabelColumn, new_label);
}

void ExpectationHolder::setRightLabelAt(int row, const utils::U8String& new_label) {
  position_to_Label[row][AttributeLayout::kRightLabelColumn] = new_label;
  checkLabelEmpty(row, AttributeLayout::kRightLabelColumn, new_label);
}

const QWidget* ExpectationHolder::widgetAt(int row, int column) const {
  return position_to_AttrView.at(row).at(column);
}

MockCommand* ExpectationHolder::commandAt(int row, int column) const {
  switch (column) {
    case AttributeLayout::kLeftAddCommandColumn:
    case AttributeLayout::kRightAddCommandColumn:
      return addCommandAt(row, column);

    case AttributeLayout::kLeftEraseCommandColumn:
    case AttributeLayout::kRightEraseCommandColumn:
      return eraseCommandAt(row, column);

    case AttributeLayout::kLeftEditCommandColumn:
    case AttributeLayout::kRightEditCommandColumn:
      return editCommandAt(row, column);

    default:
      [column]() {
        FAIL() << "column " << column << "is not a command column";
      }();
      return nullptr;  // never reach here
  }
}

MockCommand* ExpectationHolder::eraseCommandAt(int row, int column) const {
  Command* cmd = position_to_EraseCommand.at(row).at(column);
  return dynamic_cast<MockCommand*>(cmd);
}

MockCommand* ExpectationHolder::editCommandAt(int row, int column) const {
  Command* cmd = position_to_EditCommand.at(row).at(column);
  return dynamic_cast<MockCommand*>(cmd);
}

MockCommand* ExpectationHolder::addCommandAt(int row, int column) const {
  Command* cmd = position_to_AddCommand.at(row).at(column);
  return dynamic_cast<MockCommand*>(cmd);
}

bool ExpectationHolder::isGroupAt(int row, int column) const {
  if (column >= AttributeLayout::kRightLabelColumn) {
    // right side
    auto row_data = right_row_data.at(row);
    return row_data.group_block != nullptr;
  } else {
    // left side
    auto row_data = left_row_data.at(row);
    return row_data.group_block != nullptr;
  }
}

void ExpectationHolder::addLeftRowData(int row, AttributeGroupDisplayBlock* group_block) {
  left_row_data[row] = RowData(group_block);
}

void ExpectationHolder::addLeftRowData(int row, AttributeViewDisplayBlock* attr_block) {
  left_row_data[row] = RowData(attr_block);
}

void ExpectationHolder::addRightRowData(int row, AttributeGroupDisplayBlock* group_block) {
  right_row_data[row] = RowData(group_block);
}

void ExpectationHolder::addRightRowData(int row, AttributeViewDisplayBlock* attr_block) {
  right_row_data[row] = RowData(attr_block);
}

#define CONTENT_AT(side, type, value)                                   \
  position_to_##type[row][AttributeLayout::k##side##type##Column] = (value); \
  check##type##Empty(row, AttributeLayout::k##side##type##Column, (value));

// NOTE: althouth there are some code duplicate, but the following code
// is self-checkable code, by using the CONTENT_AT macro, you won't miss use
// AttrView column with position_to_Label, except the commands, you also won't
// miss use the values, it's type-checked by compiler
void ExpectationHolder::setLeftExpectation(int row, AttributeViewDisplayBlock* attr_block) {
  if (row < 0)
    return;

  if (!attr_block)
    return;

  CONTENT_AT(Left, Label,         attr_block->label);
  CONTENT_AT(Left, AttrView,      attr_block->attr_view->getWidget());
  CONTENT_AT(Left, EraseCommand,  attr_block->erase_command);
  CONTENT_AT(Left, EditCommand,   attr_block->edit_command);

  addLeftRowData(row, attr_block);
}

void ExpectationHolder::setLeftExpectation(int row, AttributeGroupDisplayBlock* group_block) {
  if (row < 0)
    return;

  if (!group_block)
    return;

  CONTENT_AT(Left, Label, group_block->label);
  CONTENT_AT(Left, AddCommand, group_block->add_command);

  addLeftRowData(row, group_block);
}

void ExpectationHolder::setRightExpectation(int row, AttributeViewDisplayBlock* attr_block) {
  if (row < 0)
    return;

  if (!attr_block)
    return;

  CONTENT_AT(Right, Label,        attr_block->label);
  CONTENT_AT(Right, AttrView,     attr_block->attr_view->getWidget());
  CONTENT_AT(Right, EraseCommand, attr_block->erase_command);
  CONTENT_AT(Right, EditCommand,  attr_block->edit_command);

  addRightRowData(row, attr_block);
}

void ExpectationHolder::setRightExpectation(int row, AttributeGroupDisplayBlock* group_block) {
  if (row < 0)
    return;

  if (!group_block)
    return;

  CONTENT_AT(Right, Label, group_block->label);
  CONTENT_AT(Right, AddCommand, group_block->add_command);

  addRightRowData(row, group_block);
}

#undef CONTENT_AT

void ExpectationHolder::checkLabelEmpty(int row, int column, const utils::U8String& label) {
  if (!label.empty())
    position_empty[row][column] = false;
  else
    position_empty[row][column] = true;
}

void ExpectationHolder::checkAttrViewEmpty(int row, int column, const QWidget* widget) {
  if (widget != nullptr)
    position_empty[row][column] = false;
}

void ExpectationHolder::checkEraseCommandEmpty(int row, int column, const Command* cmd) {
  if (cmd != nullptr)
    position_empty[row][column] = false;
}

void ExpectationHolder::checkEditCommandEmpty(int row, int column, const Command* cmd) {
  if (cmd != nullptr)
    position_empty[row][column] = false;
}

void ExpectationHolder::checkAddCommandEmpty(int row, int column, const Command* cmd) {
  if (cmd != nullptr)
    position_empty[row][column] = false;
}

void ExpectationHolder::checkAttrBlockExist(const AttributeViewDisplayBlock* attr_block) {
  if (!attr_block)
    return;

  for (auto & value : left_row_data) {
    ASSERT_NE(attr_block, value.second.attr_block)
        << "attr_block " << attr_block->label << "@" << attr_block
        << " is already added to left row" << value.first;
  }

  for (auto & value : right_row_data) {
    ASSERT_NE(attr_block, value.second.attr_block)
        << "attr_block " << attr_block->label << "@" << attr_block
        << " is already added to right row" << value.first;
  }
}

void ExpectationHolder::checkGroupBlockExist(const AttributeGroupDisplayBlock* group_block) {
  if (!group_block)
    return;

  for (auto & value : left_row_data) {
    ASSERT_NE(group_block, value.second.group_block)
        << "group_block " << group_block->label << "@" << group_block
        << " is already added to left row" << value.first;
  }

  for (auto & value : right_row_data) {
    ASSERT_NE(group_block, value.second.group_block)
        << "group_block " << group_block->label << "@" << group_block
        << " is already added to right row" << value.first;
  }
}

void ExpectationHolder::checkRowExist(int row) {
  for (auto & value : left_row_data) {
    ASSERT_NE(row, value.first)
        << "row " << row << " is already added";
  }

  for (auto & value : right_row_data) {
    ASSERT_NE(row, value.first)
        << "row " << row << " is already added";
  }
}

ExpectationHolder::RowData ExpectationHolder::indexToRowData(int index) const {
  initializeIndexToRowData();
  return index_to_row_data[index];
}

void ExpectationHolder::initializeIndexToRowData() const {
  if (!index_to_row_data_initialized) {
    checkRowData(left_row_data);
    checkRowData(right_row_data);

    int left_row_count = left_row_data.size();
    for (int index = 0; index < left_row_count; ++index) {
      index_to_row_data[index] = left_row_data.at(index);
    }

    int right_row_count = right_row_data.size();
    for (int index = 0; index < right_row_count; ++index) {
      index_to_row_data[index + left_row_count] = right_row_data.at(index);
    }

    checkIndexToRowData();

    index_to_row_data_initialized = true;
  }
}

void ExpectationHolder::checkRowData(const std::map<int, RowData>& row_data) const {
  // check the rows are continue
  std::vector<int> rows;
  for (auto & value : row_data) {
    rows.push_back(value.first);
  }

  std::sort(rows.begin(), rows.end());

  int prev_row = -1;
  for (size_t i = 0; i < rows.size(); ++i) {
    if (prev_row == -1) {
      prev_row = rows[i];

      ASSERT_EQ(0, prev_row)
          << "row 0 must have something if left side is not empty";
    } else {
      int expect_row = prev_row + 1;
      int actual_row = rows[i];

      ASSERT_EQ(expect_row, actual_row)
          << "rows is not continuous, expect: " << expect_row
          << ", actual: " << actual_row;

      prev_row = rows[i];
    }
  }
}

void ExpectationHolder::checkIndexToRowData() const {
  int count = index_to_row_data.size();
  RowData prev_group;
  int sub_attr_count = 0;

  for (int index = 0; index < count; ++index) {
    auto row_data = index_to_row_data.at(index);

    ASSERT_FALSE(row_data.isInvalid());

    if (row_data.attr_block && row_data.attr_block->is_in_group) {
      ASSERT_FALSE(prev_group.isInvalid())
          << "in group should realy in group";
      ++ sub_attr_count;
    } else {
      if (sub_attr_count) {
        ASSERT_EQ(sub_attr_count, prev_group.group_block->sub_attr_count)
            << "sub attr count in group " << prev_group.group_block
            << " should be " << sub_attr_count
            << " but is " << prev_group.group_block->sub_attr_count;
        sub_attr_count = 0;
      }

      if (row_data.group_block) {
        prev_group = row_data;
      } else {
        prev_group = RowData();
      }
    }

    if (index == (count - 1)) {
      if (sub_attr_count) {
        ASSERT_EQ(sub_attr_count, prev_group.group_block->sub_attr_count)
            << "sub attr count in group " << prev_group.group_block
            << " should be " << sub_attr_count
            << " but is " << prev_group.group_block->sub_attr_count;
      }
    }
  }  // for index

}
