// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_NAMED_STRING_FORMATTER_FACTORY_H_
#define MOCK_NAMED_STRING_FORMATTER_FACTORY_H_

#include "utils/text/i_named_string_formatter_factory.h"

#define INTERFACE_MOCK_PHASE
#include "test/interface.h"

namespace utils {
namespace text {
namespace tests {

class MockNamedStringFormatterFactory
    : public INamedStringFormatterFactory {
 public:
  INamedStringFormatterFactory_METHODS
};

}  // namespace tests
}  // namespace text
}  // namespace utils

#undef INTERFACE_MOCK_PHASE

#endif  // MOCK_NAMED_STRING_FORMATTER_FACTORY_H_
