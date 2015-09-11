// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_ATTRIBUTE_DELEGATE_H_
#define INCLUDE_QTUI_I_ATTRIBUTE_DELEGATE_H_

#include "utils/signal_slot.h"

namespace snailcore {
class IAttribute;
}  // namespace snailcore

using snailcore::IAttribute;

class IAttributeEditorView;

class IAttributeDelegate {
 public:
  virtual ~IAttributeDelegate() = default;

  SNAIL_SIGSLOT2(CreateEditorFor,
                 IAttributeEditorView*(IAttribute* attr));
  SNAIL_SIGSLOT2(CloseEditor,
                 void(IAttributeEditorView*));
};

#endif  // INCLUDE_QTUI_I_ATTRIBUTE_DELEGATE_H_
