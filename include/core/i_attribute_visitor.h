// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_ATTRIBUTE_VISITOR_H_
#define INCLUDE_CORE_I_ATTRIBUTE_VISITOR_H_

#include "include/config.h"

namespace snailcore {

FTO_BEGIN_NAMESPACE
class KbNodeAttribute;
FTO_END_NAMESPACE

class IAttributeVisitor {
 public:
  virtual ~IAttributeVisitor() = default;

  virtual void visit(fto::KbNodeAttribute* attr) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_ATTRIBUTE_VISITOR_H_
