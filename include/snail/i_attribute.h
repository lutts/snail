// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ATTRIBUTE_H_
#define INCLUDE_SNAIL_I_ATTRIBUTE_H_

namespace snailcore {

class IAttribute {
 public:
  virtual ~IAttribute() = default;

  virtual IAttribute* clone() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ATTRIBUTE_H_
