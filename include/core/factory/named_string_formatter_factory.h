// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef NAMED_STRING_FORMATTER_FACTORY_H_
#define NAMED_STRING_FORMATTER_FACTORY_H_

namespace utils {
namespace text {

FTO_BEGIN_NAMESPACE
class NamedStringFormatter;
FTO_END_NAMESPACE

class NamedStringFormatterFactory {
 public:
  virtual ~NamedStringFormatterFactory() = default;

  virtual fto::NamedStringFormatter* create() const = 0;
};

}  // namespace text
}  // namespace utils

#endif  // NAMED_STRING_FORMATTER_FACTORY_H_
