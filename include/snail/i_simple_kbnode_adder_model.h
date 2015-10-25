// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_SIMPLE_KBNODE_ADDER_MODEL_H_
#define INCLUDE_SNAIL_I_SIMPLE_KBNODE_ADDER_MODEL_H_

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IKbNode;
class ITreeItemProvider;

class ISimpleKbNodeAdderModel : public pfmvp::IPfModel {
 public:
  virtual ~ISimpleKbNodeAdderModel() = default;

  DEF_MODEL_ID(SimpleKbNodeAdderModel);

  virtual utils::U8String name() const = 0;
  virtual ITreeItemProvider* getKbNodeProvider() const = 0;
  virtual utils::U8String getDefaultNewKbNodeName() const = 0;
  virtual IKbNode* getNewKbNodeParent() const = 0;
  virtual void setNewKbNodeParent(IKbNode* kbnode) = 0;
  virtual void setNewKbNodeName(const utils::U8String& name) = 0;
  virtual bool isNewKbNodeNameValid() const = 0;
  virtual void setIsCategory(bool category) = 0;
  virtual void addKbNode() = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_SIMPLE_KBNODE_ADDER_MODEL_H_
