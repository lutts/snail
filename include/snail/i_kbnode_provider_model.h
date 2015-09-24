// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_PROVIDER_MODEL_H_
#define I_KBNODE_PROVIDER_MODEL_H_

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IKbNodeProviderModel : public pfmvp::IPfModel {
 public:
  virtual ~IKbNodeProviderModel() = default;

  DEF_MODEL_ID(KbNodeProviderModel);

  SNAIL_SIGSLOT2(KbNodeAdded,
                 void(IKbNode* new_kbnode, IKbNode* parent_kbnode));
};

}  // namespace snailcore

#endif  // I_KBNODE_PROVIDER_MODEL_H_
