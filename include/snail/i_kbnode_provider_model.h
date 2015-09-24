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

class IKbNode;
class IKbNodeProvider;

class IKbNodeProviderModel : public pfmvp::IPfModel {
 public:
  virtual ~IKbNodeProviderModel() = default;

  DEF_MODEL_ID(KbNodeProviderModel);

  SNAIL_SIGSLOT2(KbNodeAdded,
                 void(IKbNode* new_kbnode, IKbNode* parent_kbnode));

  virtual IKbNodeProvider* getKbNodeProvider() const = 0;
  virtual utils::U8String getFilterPattern() const = 0;
  virtual void setFilterPattern(const utils::U8String& filter_pattern) = 0;
  virtual IKbNode* getNewKbNodeParent() const = 0;
  virtual void setNewKbNodeParent(IKbNode* kbnode) = 0;
  virtual void setNewKbNodeName(const utils::U8String& name) = 0;
  virtual bool isNewKbNodeNameValid() const = 0;
  virtual void addKbNode() = 0;
};

}  // namespace snailcore

#endif  // I_KBNODE_PROVIDER_MODEL_H_
