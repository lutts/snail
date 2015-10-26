// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
#define INCLUDE_SNAIL_I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_

#include <memory>

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace snailcore {

class ITreeItem;
class ITreeItemProvider;
class IAttributeModel;
class IAttributeSetModel;

class IKbNodeLinkAttributePopupEditorModel : public pfmvp::IPfModel {
 public:
  virtual ~IKbNodeLinkAttributePopupEditorModel() = default;

  DEF_MODEL_ID(KbNodeLinkAttributePopupEditorModel);

  SNAIL_SIGSLOT_PURE_VIRTUAL(
      LinkTypeChanged,
      void(std::shared_ptr<IAttributeSetModel> new_attr_set_model,
           IAttributeSetModel* old_attr_set_model));
  SNAIL_SIGSLOT_PURE_VIRTUAL(ValidateComplete, void(bool result));

  virtual std::shared_ptr<IAttributeModel> createValueAttrModel() = 0;
  virtual ITreeItemProvider* getLinkTypeItemProvider() const = 0;
  virtual ITreeItem* getCurrentLinkType() const = 0;
  virtual std::shared_ptr<IAttributeSetModel> getCurrentLinkAttrSetModel() = 0;
  virtual void setLinkType(ITreeItem* link_type_item) = 0;
  virtual void editFinished() = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
