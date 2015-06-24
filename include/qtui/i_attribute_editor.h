// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_ATTRIBUTE_EDITOR_H_
#define I_ATTRIBUTE_EDITOR_H_

#include "src/qtui/generic_view_base.h"
#include "utils/signal_slot.h"

class IAttributeEditor : public GenericViewBase {
 public:
  virtual ~IAttributeEditor() = default;
};


#endif  // I_ATTRIBUTE_EDITOR_H_
