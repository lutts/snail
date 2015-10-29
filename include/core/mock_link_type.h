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

namespace snailcore {
namespace tests {

class MockLinkType : public fto::LinkType {
 public:
  // ITreeItem
  MOCK_CONST_METHOD0(name, utils::U8String());
  MOCK_CONST_METHOD0(isGroupOnly, bool());

  // self
  MOCK_CONST_METHOD0(clone, fto::LinkType*());
  MOCK_CONST_METHOD0(attributeSuppliers, std::vector<IAttributeSupplier*>());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_LINK_TYPE_H_
