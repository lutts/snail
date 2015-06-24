// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_ATTRIBUTE_H_
#define I_ATTRIBUTE_H_

namespace snailcore {

class IAttribute {
 public:
  virtual ~IAttribute() = default;

  virtual IAttribute* clone() const = 0;
};

}  // namespace snailcore

#endif  // I_ATTRIBUTE_H_
