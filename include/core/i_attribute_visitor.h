// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_ATTRIBUTE_VISITOR_H_
#define INCLUDE_CORE_I_ATTRIBUTE_VISITOR_H_

namespace snailcore {

class KbNodeAttribute;

class IAttributeVisitor {
 public:
  virtual ~IAttributeVisitor() = default;

  virtual void visit(KbNodeAttribute* attr) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_ATTRIBUTE_VISITOR_H_
