// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_WORK_H_
#define INCLUDE_CORE_MOCK_WORK_H_

#include <vector>

#include "core/fto_work.h"

namespace snailcore {
namespace tests {

class MockWork : public fto::Work {
 public:
  SNAIL_MOCK_SLOT(NameChanged);

  MOCK_METHOD1(set_name, bool(const utils::U8String& name));
  MOCK_CONST_METHOD0(name, utils::U8String());

  MOCK_CONST_METHOD0(attributeSuppliers, std::vector<IAttributeSupplier*>());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_WORK_H_
