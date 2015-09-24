// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_PROVIDER_VIEW_H_
#define I_KBNODE_PROVIDER_VIEW_H_

#include "src/qtui/generic_view_base.h"
#include "utils/signal_slot.h"

class IKbNodeProviderView : public GenericViewBase {
 public:
  virtual ~IKbNodeProviderView() = default;
};


#endif  // I_KBNODE_PROVIDER_VIEW_H_
