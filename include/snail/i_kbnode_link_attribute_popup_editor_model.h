// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
#define I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_

#include <memory>

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IKbNode;
class IKbNodeProvider;
class IAttributeModel;
class IAttributeSetModel;

class IKbNodeLinkAttributePopupEditorModel : public pfmvp::IPfModel {
 public:
  virtual ~IKbNodeLinkAttributePopupEditorModel() = default;

  DEF_MODEL_ID(KbNodeLinkAttributePopupEditorModel);

  SNAIL_SIGSLOT2(LinkTypeChanged,
                 void(std::shared_ptr<IAttributeSetModel> attr_set_model));
  SNAIL_SIGSLOT2(ValidateComplete, void(bool result));

  virtual std::shared_ptr<IAttributeModel> createValueKbNodeAttrModel() = 0;
  virtual IKbNodeProvider* getLinkTypeProvider() const = 0;
  virtual IKbNode* getCurrentLinkType() const = 0;
  virtual std::shared_ptr<IAttributeSetModel> getCurrentLinkAttrSetModel() = 0;
  virtual void setLinkType(IKbNode* link_type) = 0;
  virtual void editFinished() = 0;
};

}  // namespace snailcore

#endif  // I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
