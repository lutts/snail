// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_QTUI_TEST_ATTR_DISPLAY_BLOCK_GENERATOR_H_
#define TEST_QTUI_TEST_ATTR_DISPLAY_BLOCK_GENERATOR_H_

#include <memory>
#include <map>

#include "utils/basic_utils.h"
#include "utils/u8string.h"

class AttributeViewDisplayBlock;
class QWidget;

namespace snailcore {
class AttributeGroupDisplayBlock;
}  // namespace snailcore

namespace utils {
class Command;

namespace tests {
class MockCommand;
}  // namespace tests
}  // namespace utils


class TestAttributePoolImpl;

class TestAttributePool {
 public:
  TestAttributePool();
  virtual ~TestAttributePool();

  AttributeViewDisplayBlock* createAttr();
  AttributeViewDisplayBlock* createAttr(const utils::U8String& label);

  AttributeViewDisplayBlock* createAttr(
      snailcore::AttributeGroupDisplayBlock* owner_group);
  AttributeViewDisplayBlock* createAttr(
      snailcore::AttributeGroupDisplayBlock* owner_group,
      const utils::U8String& label);

  snailcore::AttributeGroupDisplayBlock* createGroup();
  snailcore::AttributeGroupDisplayBlock* createGroup(
      const utils::U8String& label);

  void associateAttributeToGroup(
      AttributeViewDisplayBlock* attr_block,
      snailcore::AttributeGroupDisplayBlock* attr_group);
  void verifyMocks();

 private:
  std::unique_ptr<TestAttributePoolImpl> impl;
  friend class TestAttributePoolImpl;

 private:
  SNAIL_DISABLE_COPY(TestAttributePool)
};

class ExpectationHolder {
 public:
  ExpectationHolder() { }
  virtual ~ExpectationHolder() = default;

  // begin of methods for test cases
  void setExpectAA(int row,
                 AttributeViewDisplayBlock* left_side_attr,
                 AttributeViewDisplayBlock* right_side_attr);

  void setExpectGA(int row,
                 snailcore::AttributeGroupDisplayBlock* left_group,
                 AttributeViewDisplayBlock* right_side_attr);

  void setExpectAG(int row,
                 AttributeViewDisplayBlock* left_side_attr,
                 snailcore::AttributeGroupDisplayBlock* right_group);

  void setExpectGG(int row,
                 snailcore::AttributeGroupDisplayBlock* left_group,
                 snailcore::AttributeGroupDisplayBlock* right_group);
  // end of methods for test cases

  // begin of methods for layout
  int num_attrs() const;
  AttributeViewDisplayBlock* attrViewBlockAt(int index) const;
  bool isGroupAt(int index) const;
  snailcore::AttributeGroupDisplayBlock* attrGroupBlockAt(int index) const;
  // end of method for layout

  // begin of methods for verify result
  int rowCount() const;
  bool isEmpty(int row, int column) const;
  void setEmpty(int row, int column);
  utils::U8String labelAt(int row, int column) const;
  // utils::U8String labelAt(int index) const;
  void setLeftLabelAt(int row, const utils::U8String& new_label);
  void setRightLabelAt(int row, const utils::U8String& new_label);
  const QWidget* widgetAt(int row, int column) const;
  utils::tests::MockCommand* commandAt(int row, int column) const;
  utils::tests::MockCommand* addCommandAt(int row, int column) const;
  bool isGroupAt(int row, int column) const;
  // end of methods for verify result

 private:
  class RowData {
   public:
    RowData() = default;
    explicit RowData(snailcore::AttributeGroupDisplayBlock* group)
        : group_block(group) { }
    explicit RowData(AttributeViewDisplayBlock* attr)
        : attr_block(attr) { }

    snailcore::AttributeGroupDisplayBlock* group_block { nullptr };
    AttributeViewDisplayBlock* attr_block { nullptr };

    bool isInvalid() {
      return ((group_block == nullptr) && (attr_block == nullptr)) ||
          ((group_block != nullptr) && (attr_block != nullptr));
    }
  };

  void addLeftRowData(int row,
                      snailcore::AttributeGroupDisplayBlock* group_block);
  void addLeftRowData(int row, AttributeViewDisplayBlock* attr_block);
  void addRightRowData(int row,
                       snailcore::AttributeGroupDisplayBlock* group_block);
  void addRightRowData(int row, AttributeViewDisplayBlock* attr_block);

  void setLeftExpectation(int row, AttributeViewDisplayBlock* attr_block);
  void setLeftExpectation(int row,
                          snailcore::AttributeGroupDisplayBlock* group_block);
  void setRightExpectation(int row, AttributeViewDisplayBlock* attr_block);
  void setRightExpectation(int row,
                           snailcore::AttributeGroupDisplayBlock* group_block);

  void checkLabelEmpty(int row, int column, const utils::U8String& label);
  void checkAttrViewEmpty(int row, int column, const QWidget* widget);
  void checkAddCommandEmpty(int row, int column, const utils::Command* cmd);
  void checkAttrBlockExist(const AttributeViewDisplayBlock* attr_block);
  void checkGroupBlockExist(
      const snailcore::AttributeGroupDisplayBlock* group_block);
  void checkRowExist(int row);

  RowData indexToRowData(int index) const;
  void initializeIndexToRowData() const;
  void checkRowData(const std::map<int, RowData>& row_data) const;
  void checkIndexToRowData() const;

  std::map<int, RowData> left_row_data;
  std::map<int, RowData> right_row_data;
  mutable std::map<int, RowData> index_to_row_data;
  mutable bool index_to_row_data_initialized { false };

  // expections
  std::map<int, std::map<int, utils::U8String> > position_to_Label;
  std::map<int, std::map<int, QWidget*> > position_to_AttrView;
  std::map<int, std::map<int, utils::Command*> > position_to_AddCommand;

  std::map<int, std::map<int, bool> > position_empty;

 private:
  SNAIL_DISABLE_COPY(ExpectationHolder)
};

#endif  // TEST_QTUI_TEST_ATTR_DISPLAY_BLOCK_GENERATOR_H_
