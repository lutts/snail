// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_KBNODE_ATTRIBUTE_MODEL_H_
#define INCLUDE_SNAIL_I_KBNODE_ATTRIBUTE_MODEL_H_

#include "snail/i_attribute_model.h"

namespace snailcore {

class IKbNode;
class ITreeItemProvider;

class ISimpleKbNodeAdderModel;

class IKbNodeAttributeModel : public IAttributeModel {
 public:
  enum {
    kSetKbNodeSuccess = 0,
    kSetKbNodeNotFound,
    kSetKbNodeMultpicMatched,
  };

  virtual ~IKbNodeAttributeModel() = default;

  DEF_MODEL_ID(KbNodeAttributeModel);

  virtual ITreeItemProvider* getKbNodeProvider() const = 0;

  virtual std::shared_ptr<ISimpleKbNodeAdderModel>
  createSimpleKbNodeAdderModel() const = 0;

  virtual utils::U8String getKbNodeName() const = 0;
  virtual void setKbNode(IKbNode* kbnode) = 0;
  virtual int setKbNodeByName(const utils::U8String& name) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_KBNODE_ATTRIBUTE_MODEL_H_
