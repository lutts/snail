// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_ATTRIBUTE_VISITOR_H_
#define I_ATTRIBUTE_VISITOR_H_

namespace snailcore {

class IKbNodeAttribute;

class IAttributeVisitor {
 public:
  virtual ~IAttributeVisitor() = default;

  virtual void visit(IKbNodeAttribute* attr) = 0;
};

}  // namespace snailcore

#endif  // I_ATTRIBUTE_VISITOR_H_
