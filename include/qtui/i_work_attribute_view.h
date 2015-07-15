// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_WORK_ATTRIBUTE_VIEW_H_
#define I_WORK_ATTRIBUTE_VIEW_H_

#include "src/qtui/generic_view_base.h"
#include "utils/signal_slot.h"

class IAttributeView;
namespace utils {
class Command;
}  // namespace utils

class IWorkAttributeView : public GenericViewBase {
 public:
  virtual ~IWorkAttributeView() = default;

  SNAIL_SIGSLOT2(EditModeButtonClicked, void());
  SNAIL_SIGSLOT2(DoneButtonClicked, void());

  virtual void beginReLayoutAttributes() = 0;
  virtual void endReLayoutAttributes() = 0;

  virtual void addLabel(const utils::U8String& label,
                        int row, int column, int row_span, int column_span) = 0;
  virtual void addAttribute(IAttributeView* attr_view,
                            int row, int column,
                            int row_span, int column_span) = 0;
  virtual void addEraseCommand(utils::Command* command,
                               int row, int column,
                               int row_span, int column_span) = 0;
  virtual void addPopupEditorCommand(utils::Command* command,
                                     int row, int column,
                                     int row_span, int column_span) = 0;
  virtual void addAddAttributeCommand(utils::Command* command,
                                      int row, int column,
                                      int row_span, int column_span) = 0;
};


#endif  // I_WORK_ATTRIBUTE_VIEW_H_
