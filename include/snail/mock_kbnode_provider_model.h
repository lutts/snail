// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_PROVIDER_MODEL_H_
#define MOCK_KBNODE_PROVIDER_MODEL_H_

#include "snail/i_kbnode_provider_model.h"

namespace snailcore {
namespace tests {

class MockKbNodeProviderModel : public IKbNodeProviderModel {
 public:
  SNAIL_MOCK_SLOT(KbNodeAdded);
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_PROVIDER_MODEL_H_
