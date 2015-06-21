// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_QTUI_WORK_TEST_UTILS_H_
#define TEST_QTUI_WORK_TEST_UTILS_H_

#include <memory>
#include "snail/mock_work_model.h"      // MockWork
#include "utils/u8string.h"   // U8String
#include "test/testutils/utils.h"  // xtestutils

using snailcore::tests::MockWorkModel;

static std::shared_ptr<MockWorkModel> createNamedWork() {
  auto work = std::make_shared<MockWorkModel>();
  utils::U8String workName = xtestutils::genRandomString();
  ON_CALL(*work, name()).WillByDefault(Return(workName));
  return work;
}

#endif  // TEST_QTUI_WORK_TEST_UTILS_H_
