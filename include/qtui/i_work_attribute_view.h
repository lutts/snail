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

namespace utils {
class Command;
}  // namespace utils

namespace snailcore {
class AttributeGroupDisplayBlock;
}  // namespace snailcore

class IAttributeView;

struct AttributeViewDisplayBlock {
  utils::U8String label;
  IAttributeView* attr_view;
  utils::Command* erase_command;
  utils::Command* edit_command;
  bool is_in_group;
};

class IAttributeLayout {
 public:
  virtual ~IAttributeLayout() = default;

  virtual void beginAddAttributeDisplayBlock(int total_block_count) = 0;
  virtual void addAttributeGroupDisplayBlock(
      snailcore::AttributeGroupDisplayBlock* attr_group_block) = 0;
  virtual void addAttributeDisplayBlock(
      AttributeViewDisplayBlock attr_view_block) = 0;
  virtual void endAddAttributeDisplayBlock() = 0;
};

class IWorkAttributeView : public GenericViewBase {
 public:
  virtual ~IWorkAttributeView() = default;

  SNAIL_SIGSLOT2(EditModeButtonClicked, void());
  SNAIL_SIGSLOT2(DoneButtonClicked, void());
};


#endif  // INCLUDE_QTUI_I_WORK_ATTRIBUTE_VIEW_H_
