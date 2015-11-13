// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_NAMED_STRING_FORMATTER_H_
#define MOCK_NAMED_STRING_FORMATTER_H_

#include "utils/text/fto_named_string_formatter.h"

#define INTERFACE_MOCK_PHASE
#include "test/interface.h"

namespace utils {
namespace text {
namespace tests {

class MockNamedStringFormatter : public fto::NamedStringFormatter {
 public:
  NamedStringFormatter_METHODS
};

class MockNamedStringFormatterFactory
    : public fto::NamedStringFormatterFactory {
 public:
  NamedStringFormatterFactory_METHODS
};

}  // namespace tests
}  // namespace text
}  // namespace utils

#undef INTERFACE_MOCK_PHASE

#endif  // MOCK_NAMED_STRING_FORMATTER_H_
