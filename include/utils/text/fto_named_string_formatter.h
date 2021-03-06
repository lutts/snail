// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_TEXT_FTO_NAMED_STRING_FORMATTER_H_
#define INCLUDE_UTILS_TEXT_FTO_NAMED_STRING_FORMATTER_H_

// region: shared with impl

#include "include/config.h"
#include "utils/u8string.h"

namespace utils {
namespace text {

class VariableResolver;

}  // namespace text
}  // namespace utils

// endregion: shared

// region: Interface

#ifndef DISABLE_TEST_CODE

#include "test/test_proxy.h"

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

namespace utils {
namespace text {
namespace fto {

#define NamedStringFormatter_METHODS                                      \
  SNAIL_CONST_INTERFACE2(                                                 \
      format, utils::U8String(const utils::U8String& named_format_string, \
                              const VariableResolver* variable_resolver));

class NamedStringFormatter {
 public:
  virtual ~NamedStringFormatter() = default;

  TEST_PROXY_CLONE_INTERFACE(NamedStringFormatter);
  TEST_PROXY_MOVE_CLONE_INTERFACE(NamedStringFormatter);
  TEST_PROXY_MOVE_ASSIGN_INTERFACE(NamedStringFormatter);
  TEST_PROXY_SWAP_INTERFACE(NamedStringFormatter);

  NamedStringFormatter_METHODS
};

}  // namespace fto
}  // namespace text
}  // namespace utils

#undef INTERFACE_DEFINITION_PHASE

#else  // DISABLE_TEST_CODE

namespace utils {
namespace text {
namespace fto {

class NamedStringFormatter {};

}  // namespace fto
}  // namespace text
}  // namespace utils

#endif  // DISABLE_TEST_CODE
// endregion: Interface

// region: TestProxy
#ifndef DISABLE_TEST_CODE

#define INTERFACE_TEST_PROXY_PHASE
#include "test/interface.h"
#include "core/factory/named_string_formatter_factory.h"

namespace utils {
namespace text {
namespace fto {

class NamedStringFormatterTestProxy {
  TEST_PROXY_BASE(NamedStringFormatter);
  TEST_PROXY_ENABLE_COPY_CONSTRUCT(NamedStringFormatter);
  TEST_PROXY_DISABLE_COPY_ASSIGNMENT(NamedStringFormatter);
  TEST_PROXY_ENABLE_MOVE_CONSTRUCT(NamedStringFormatter);
  TEST_PROXY_DISABLE_MOVE_ASSIGNMENT(NamedStringFormatter);
  TEST_PROXY_ENABLE_SWAP(NamedStringFormatter);
  TEST_PROXY_ENABLE_FACTORY_SUPPORT(NamedStringFormatter);

 public:
  NamedStringFormatterTestProxy() { create(); }

  NamedStringFormatter_METHODS
};

}  // namespace fto
}  // namespace text
}  // namespace utils

#undef INTERFACE_TEST_PROXY_PHASE
#endif  // DISABLE_TEST_CODE

// endregion: TestProxy

#endif  // INCLUDE_UTILS_TEXT_FTO_NAMED_STRING_FORMATTER_H_
