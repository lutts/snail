// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_WORK_H_
#define INCLUDE_SNAIL_MOCK_WORK_H_

#include "snail/i_work.h"

namespace snailcore {
namespace tests {

class MockWork : public IWork {
 public:
  MOCK_METHOD1(set_name, void(const utils::U8String& name));
  MOCK_CONST_METHOD0(name, const utils::U8String&());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_WORK_H_
