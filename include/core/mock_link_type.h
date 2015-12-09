// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_LINK_TYPE_H_
#define INCLUDE_CORE_MOCK_LINK_TYPE_H_

#include <vector>
#include "core/fto_link_type.h"

#define INTERFACE_MOCK_PHASE
#include "test/interface.h"

namespace snailcore {
namespace tests {

class MockLinkType : public fto::LinkType {
 public:
  // ITreeItem
  MOCK_CONST_METHOD0(name, utils::U8String());
  MOCK_CONST_METHOD0(isGroupOnly, bool());

  SNAIL_MOCK_SLOT(LinkUpdated);

  SNAIL_CONST_INTERFACE0(clone, fto::LinkType*());

  fto::LinkType& operator=(const fto::LinkType& rhs) override {
    copy_assignment(rhs);
    return *this;
  }

  MOCK_METHOD1(copy_assignment, void(const fto::LinkType& rhs));

  fto::LinkType& operator=(fto::LinkType&& rhs) override {
    move_assignment(rhs);
    return *this;
  }

  MOCK_METHOD1(move_assignment, void(fto::LinkType& rhs));

  LinkType_METHODS
};

}  // namespace tests
}  // namespace snailcore

#undef INTERFACE_MOCK_PHASE

#endif  // INCLUDE_CORE_MOCK_LINK_TYPE_H_
