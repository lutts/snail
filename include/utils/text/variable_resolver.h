// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_TEXT_VARIABLE_RESOLVER_H_
#define INCLUDE_UTILS_TEXT_VARIABLE_RESOLVER_H_

#include <vector>

#include "utils/u8string.h"

namespace utils {
namespace text {

class VariableResolver {
 public:
  virtual ~VariableResolver() = default;

  virtual std::vector<U8String> lookup(const U8String& var_name) = 0;
};

}  // namespace text
}  // namespace utils

#endif  // INCLUDE_UTILS_TEXT_VARIABLE_RESOLVER_H_
