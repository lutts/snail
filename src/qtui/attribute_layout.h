// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_ATTRIBUTE_LAYOUT_H_
#define SRC_QTUI_ATTRIBUTE_LAYOUT_H_

#include <QGridLayout>

#include <forward_list>

#include "utils/basic_utils.h"
#include "qtui/i_work_attribute_view.h"

class QTimer;

class AttributeLayout : public QGridLayout
                      , public IAttributeLayout {
  Q_OBJECT

 public:
  enum {
    kLeftLabelColumn = 0,
    kLeftAttrViewColumn,
    kLeftAddCommandColumn = kLeftAttrViewColumn,

    kSeperatorColumn,

    kRightLabelColumn,
    kRightAttrViewColumn,
    kRightAddCommandColumn = kRightAttrViewColumn,

    kTotalColumn
  };

  AttributeLayout();
  virtual ~AttributeLayout() = default;

  void set_triad_manager(pfmvp::IPfTriadManager* triad_manager) override {
    triad_manager_ = triad_manager;
  }

  void beginLayout(int total_block_count) override;
  void* layoutAttributeGroupDisplayBlock(
      snailcore::AttributeGroupDisplayBlock attr_group_block) override;
  void* layoutAttributeDisplayBlock(
      AttributeViewDisplayBlock attr_view_block) override;
  void updateLabel(snailcore::UpdateAttrLabelData label_data) override;
  void endLayout(bool remove_triads) override;

 private slots:
  void clearOldWidgets(int remove_triads);

 private:
  enum AttrType {
    ATTR_TYPE_NONE,
    ATTR_TYPE_GROUP,
    ATTR_TYPE_SUB_ATTR,
    ATTR_TYPE_TOPLEVEL_ATTR,
  };

  // void addToolButton(utils::Command* command, int row, int column);
  void addPushButton(utils::Command* command, int row, int column);
  void mayAdjustLeftSideCountOnGroup(int sub_item_count);
  utils::U8String label_to_display(utils::U8String label,
                                   AttrType attr_type);

  int total_item_count { 0 };
  int left_side_count { 0 };
  int added_rows { 0 };

  AttrType last_attr_type = ATTR_TYPE_NONE;
  utils::U8String last_label { "" };

  std::forward_list<QWidget*> curr_widgets;
  std::forward_list<QWidget*> curr_attr_widgets;

  std::forward_list<QWidget*> to_be_deleted_widgets;
  std::forward_list<QWidget*> to_be_removed_widgets;

  pfmvp::IPfTriadManager* triad_manager_ { nullptr };
  QTimer *timer { nullptr };

 private:
  SNAIL_DISABLE_COPY(AttributeLayout)
};


#endif  // SRC_QTUI_ATTRIBUTE_LAYOUT_H_
