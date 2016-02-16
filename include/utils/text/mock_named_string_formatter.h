// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_TEXT_MOCK_NAMED_STRING_FORMATTER_H_
#define INCLUDE_UTILS_TEXT_MOCK_NAMED_STRING_FORMATTER_H_

#include "utils/text/fto_named_string_formatter.h"
#include "utils/weak_singleton.h"

#define INTERFACE_MOCK_PHASE
#include "test/interface.h"

namespace utils {
namespace text {
namespace tests {

class MockNamedStringFormatter : public fto::NamedStringFormatter {
 public:
  TEST_PROXY_CLONE_MOCK(fto::NamedStringFormatter);
  TEST_PROXY_MOVE_CLONE_MOCK(fto::NamedStringFormatter);
  TEST_PROXY_MOVE_ASSIGN_MOCK(fto::NamedStringFormatter);
  TEST_PROXY_SWAP_MOCK(fto::NamedStringFormatter);

  NamedStringFormatter_METHODS
};

class MockNamedStringFormatterFactory
    : public NamedStringFormatterFactory,
      public WeakSingleton<MockNamedStringFormatterFactory> {
 public:
  MockNamedStringFormatterFactory() {
    fto::NamedStringFormatterTestProxy::setFactory(this);
  }

  virtual ~MockNamedStringFormatterFactory() {
    fto::NamedStringFormatterTestProxy::setFactory(nullptr);
  }

  MOCK_CONST_METHOD0(create, fto::NamedStringFormatter*());
};

}  // namespace tests
}  // namespace text
}  // namespace utils

#undef INTERFACE_MOCK_PHASE

#endif  // INCLUDE_UTILS_TEXT_MOCK_NAMED_STRING_FORMATTER_H_
