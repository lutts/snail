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
#include "pfmvp/i_pf_triad_manager.h"

class IAttributeView;

struct AttributeViewDisplayBlock {
  AttributeViewDisplayBlock()
      : attr_view{nullptr}
      , is_in_group{false}
      , view_priv_data{nullptr} { }

  utils::U8String label;
  IAttributeView* attr_view;
  bool is_in_group;
  void* view_priv_data;
};

class IAttributeLayout {
 public:
  virtual ~IAttributeLayout() = default;

  virtual void set_triad_manager(pfmvp::IPfTriadManager* triad_manager) = 0;
  virtual void beginLayout(int total_block_count) = 0;
  virtual void* layoutAttributeGroupDisplayBlock(
      snailcore::AttributeGroupDisplayBlock attr_group_block) = 0;
  virtual void* layoutAttributeDisplayBlock(
      AttributeViewDisplayBlock attr_view_block) = 0;
  virtual void updateLabel(snailcore::UpdateAttrLabelData label_data) = 0;
  virtual void endLayout(bool remove_triads) = 0;
};

class IAttributeCollectionView : public GenericViewBase {
 public:
  virtual ~IAttributeCollectionView() = default;

  SNAIL_SIGSLOT2(EditModeButtonClicked, void());
  SNAIL_SIGSLOT2(DoneButtonClicked, void());

  virtual void setEditMode(bool edit_mode) = 0;
};


#endif  // INCLUDE_QTUI_I_WORK_ATTRIBUTE_VIEW_H_
