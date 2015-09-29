// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_KBNODE_PROVIDER_MODEL_H_
#define INCLUDE_SNAIL_MOCK_KBNODE_PROVIDER_MODEL_H_

#include "snail/i_kbnode_provider_model.h"

namespace snailcore {
namespace tests {

class MockKbNodeProviderModel : public IKbNodeProviderModel {
 public:
  SNAIL_MOCK_SLOT(KbNodeAdded);

  MOCK_CONST_METHOD0(getKbNodeProvider, IKbNodeProvider*());
  MOCK_CONST_METHOD0(getFilterPattern, utils::U8String());
  MOCK_METHOD1(setFilterPattern, void(const utils::U8String& filter_pattern));
  MOCK_CONST_METHOD0(getNewKbNodeParent, IKbNode*());
  MOCK_METHOD1(setNewKbNodeParent, void(IKbNode* kbnode));
  MOCK_METHOD1(setNewKbNodeName, void(const utils::U8String& name));
  MOCK_CONST_METHOD0(isNewKbNodeNameValid, bool());
  MOCK_METHOD1(setIsCategory, void(bool category));
  MOCK_METHOD0(addKbNode, void());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_KBNODE_PROVIDER_MODEL_H_
