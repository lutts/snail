// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_ATTRIBUTE_VISITOR_H_
#define INCLUDE_CORE_MOCK_ATTRIBUTE_VISITOR_H_

#include "core/i_attribute_visitor.h"

namespace snailcore {
namespace tests {

class MockAttributeVisitor : public IAttributeVisitor {
 public:
  MOCK_METHOD1(visit, void(IKbNodeAttribute* attr));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_ATTRIBUTE_VISITOR_H_
