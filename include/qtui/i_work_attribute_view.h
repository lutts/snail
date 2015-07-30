// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_WORK_ATTRIBUTE_VIEW_H_
#define INCLUDE_QTUI_I_WORK_ATTRIBUTE_VIEW_H_

#include "src/qtui/generic_view_base.h"
#include "utils/u8string.h"
#include "utils/signal_slot.h"
#include "snail/attribute_display_block.h"

namespace utils {
class Command;
}  // namespace utils

class IAttributeView;

struct AttributeViewDisplayBlock {
  AttributeViewDisplayBlock()
      : attr_view{nullptr}
      , erase_command{nullptr}
      , edit_command{nullptr}
      , is_in_group{false}
      , view_priv_data{nullptr} { }

  utils::U8String label;
  IAttributeView* attr_view;
  utils::Command* erase_command;
  utils::Command* edit_command;
  bool is_in_group;
  void* view_priv_data;
};

class IAttributeLayout {
 public:
  virtual ~IAttributeLayout() = default;

  virtual void beginLayout(int total_block_count) = 0;
  virtual void* layoutAttributeGroupDisplayBlock(
      snailcore::AttributeGroupDisplayBlock attr_group_block) = 0;
  virtual void* layoutAttributeDisplayBlock(
      AttributeViewDisplayBlock attr_view_block) = 0;
  virtual void endLayout() = 0;
};

class IWorkAttributeView : public GenericViewBase {
 public:
  virtual ~IWorkAttributeView() = default;

  SNAIL_SIGSLOT2(EditModeButtonClicked, void());
  SNAIL_SIGSLOT2(DoneButtonClicked, void());
};


#endif  // INCLUDE_QTUI_I_WORK_ATTRIBUTE_VIEW_H_
