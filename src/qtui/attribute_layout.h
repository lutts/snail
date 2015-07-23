// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_ATTRIBUTE_LAYOUT_H_
#define SRC_QTUI_ATTRIBUTE_LAYOUT_H_

#include <QGridLayout>

#include "utils/basic_utils.h"
#include "qtui/i_work_attribute_view.h"

class AttributeLayout : public IAttributeLayout
                      , public QGridLayout {
 public:
  enum {
    kNumColumnPerSide = 4,
    kNumSeperatorColumn = 1,
    kRightSideFirstColumn = kNumColumnPerSide + kNumSeperatorColumn,
  };

  AttributeLayout() = default;
  virtual ~AttributeLayout() = default;

  void beginAddAttributeDisplayBlock(int total_block_count) override;
  void addAttributeGroupDisplayBlock(
      snailcore::AttributeGroupDisplayBlock attr_group_block) override;
  void addAttributeDisplayBlock(
      AttributeViewDisplayBlock attr_view_block) override;
  void endAddAttributeDisplayBlock() override;

 private:
  void addToolButton(utils::Command* command, int row, int column);
  void addPushButton(utils::Command* command, int row, int column);
  void mayAdjustLeftSideCountOnGroup(int sub_item_count);
  void clear();

  int total_item_count { 0 };
  int left_side_count { 0 };
  int added_rows { 0 };

  enum AttrType {
    ATTR_TYPE_NONE,
    ATTR_TYPE_GROUP,
    ATTR_TYPE_SUB_ATTR,
    ATTR_TYPE_TOPLEVEL_ATTR,
  };

  utils::U8String label_to_display(utils::U8String label,
                                   AttrType attr_type);

  AttrType last_attr_type = ATTR_TYPE_NONE;
  utils::U8String last_label { "" };

 private:
  SNAIL_DISABLE_COPY(AttributeLayout)
};


#endif  // SRC_QTUI_ATTRIBUTE_LAYOUT_H_
