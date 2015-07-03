// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ENTITY_PROVIDER_H_
#define INCLUDE_SNAIL_I_ENTITY_PROVIDER_H_

#include <memory>
#include <utility>
#include <vector>

#include "utils/u8string.h"

namespace snailcore {

class IEntity;

class IEntityProvider {
 public:
  virtual ~IEntityProvider() = default;

  virtual std::vector<std::shared_ptr<const IEntity>> getEntities() const = 0;
  virtual std::shared_ptr<const IEntity> getDefaultEntity() const = 0;
  virtual void touchEntity(std::shared_ptr<const IEntity> entity) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ENTITY_PROVIDER_H_
