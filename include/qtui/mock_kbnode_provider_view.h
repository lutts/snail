// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_PROVIDER_VIEW_H_
#define MOCK_KBNODE_PROVIDER_VIEW_H_

#include "qtui/i_kbnode_provider_view.h"

class MockKbNodeProviderView : public IKbNodeProviderView {
 public:
  MOCK_METHOD1(showView, bool(bool));
};

#endif  // MOCK_KBNODE_PROVIDER_VIEW_H_
