// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_ATTRIBUTE_MODEL_H_
#define I_KBNODE_ATTRIBUTE_MODEL_H_

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IKbNode;
class IKbNodeProvider;

class IKbNodeAttributeModel : public pfmvp::IPfModel {
 public:
  enum {
    kSetKbNodeSuccess = 0,
    kSetKbNodeNotFound,
    kSetKbNodeMultpicMatched,
  };

  virtual ~IKbNodeAttributeModel() = default;

  DEF_MODEL_ID(KbNodeAttributeModel);

  virtual IKbNodeProvider* getKbNodeProvider() const = 0;
  virtual utils::U8String getKbNodeName() const = 0;
  virtual void setKbNode(IKbNode* kbnode) = 0;
  virtual int setKbNodeByName(const utils::U8String& name) = 0;
};

}  // namespace snailcore

#endif  // I_KBNODE_ATTRIBUTE_MODEL_H_
