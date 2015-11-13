// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_NAMED_STRING_FORMATTER_FACTORY_H_
#define I_NAMED_STRING_FORMATTER_FACTORY_H_

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

namespace utils {
namespace text {

FTO_BEGIN_NAMESPACE
class NamedStringFormatter;
FTO_END_NAMESPACE

#define INamedStringFormatterFactory_METHODS    \
  SNAIL_CONST_INTERFACE0(createInstance,        \
                         fto::NamedStringFormatter*());

class INamedStringFormatterFactory {
 public:
  virtual ~INamedStringFormatterFactory() = default;

  INamedStringFormatterFactory_METHODS
};

}  // namespace text
}  // namespace utils

#undef INTERFACE_DEFINITION_PHASE

#endif  // I_NAMED_STRING_FORMATTER_FACTORY_H_
