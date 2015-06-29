// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_FEW_ENTITY_PROVIDER_ADAPTER_H_
#define I_FEW_ENTITY_PROVIDER_ADAPTER_H_

#include "utils/signal_slot.h"
#include "snail/i_entity_provider.h"

namespace snailcore {
class IEntity;
}  // namespace snailcore

class IFewEntityCandidatesAdapter {
 public:
  virtual ~IFewEntityCandidatesAdapter() = default;

  SNAIL_SIGSLOT2(EntitySelectionChanged,
                 void(std::shared_ptr<const snailcore::IEntity> new_entity));

  virtual void setCandidateEntities(
      std::vector<std::shared_ptr<const snailcore::IEntity>> entities) = 0;
  virtual void setCurrentEntityIndex(int index) = 0;
};

#endif  // I_FEW_ENTITY_PROVIDER_ADAPTER_H_
