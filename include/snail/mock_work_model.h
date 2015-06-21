// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_WORK_MODEL_H_
#define INCLUDE_SNAIL_MOCK_WORK_MODEL_H_

#include "snail/i_work_model.h"

namespace snailcore {
namespace tests {

class MockWorkModel : public IWorkModel {
 public:
  SNAIL_MOCK_SLOT(BasicInfoChanged);

  MOCK_CONST_METHOD0(name, utils::U8String());
  MOCK_METHOD1(set_name, bool(const utils::U8String& new_name));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_WORK_MODEL_H_
