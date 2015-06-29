// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ATTRIBUTE_H_
#define INCLUDE_SNAIL_I_ATTRIBUTE_H_

#include <memory>

namespace snailcore {

class IEntityProvider;
class IEntity;

class IAttribute {
 public:
  virtual ~IAttribute() = default;

  virtual void setEntity(std::shared_ptr<const IEntity> entity) = 0;
  virtual std::shared_ptr<const IEntity> getEntity() const = 0;

  virtual IAttribute* clone() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ATTRIBUTE_H_
