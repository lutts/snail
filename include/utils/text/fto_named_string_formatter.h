// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef FTO_NAMED_STRING_FORMATER_H_
#define FTO_NAMED_STRING_FORMATER_H_

#include "include/config.h"
#include "test/test_proxy.h"
#include "utils/u8string.h"
#include "utils/text/i_named_string_formatter_factory.h"

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

namespace utils {
namespace text {

class VariableResolver;

namespace fto {

#ifndef DISABLE_TEST_CODE

#define NamedStringFormatter_METHODS                                    \
  SNAIL_CONST_INTERFACE2(                                               \
      format, utils::U8String(const utils::U8String& named_format_string, \
                              const VariableResolver* variable_resolver));

class NamedStringFormatter {
 public:
  virtual ~NamedStringFormatter() = default;

  NamedStringFormatter_METHODS
};

#else  // DISABLE_TEST_CODE

class NamedStringFormatter { };

#endif  // DISABLE_TEST_CODE

}  // namespace fto
}  // namespace text
}  // namespace utils

#undef INTERFACE_DEFINITION_PHASE

#ifndef DISABLE_TEST_CODE
#define INTERFACE_TEST_PROXY_PHASE
#include "test/interface.h"

namespace utils {
namespace text {
namespace fto {

class NamedStringFormatterTestProxy {
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(NamedStringFormatter);
  TEST_PROXY_ENABLE_FACTORY_SUPPORT(INamedStringFormatter);

 public:
  // default constructor
  NamedStringFormatterTestProxy() {
    setSelf(getFactory()->createInstance());
  }

  NamedStringFormatter_METHODS
};

}  // namespace fto
}  // namespace text
}  // namespace utils

#undef INTERFACE_TEST_PROXY_PHASE
#endif  // DISABLE_TEST_CODE


#endif  // FTO_NAMED_STRING_FORMATER_H_
