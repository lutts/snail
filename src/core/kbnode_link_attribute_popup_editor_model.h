// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
#define KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_

#include "snail/i_kbnode_link_attribute_popup_editor_model.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class IAttribute;
class IKbNodeLinkAttribute;
class IAttributeModelFactory;

class KbNodeLinkAttributePopupEditorModel
    : public IKbNodeLinkAttributePopupEditorModel {
 public:
  KbNodeLinkAttributePopupEditorModel(
      IKbNodeLinkAttribute* attr,
      IAttributeModelFactory* attr_model_factory);
  virtual ~KbNodeLinkAttributePopupEditorModel();

  std::shared_ptr<IAttributeModel> createValueKbNodeAttrModel() override;
  IKbNodeProvider* getLinkTypeProvider() const override;
  IKbNode* getCurrentLinkType() const override;
  std::shared_ptr<IAttributeSetModel> getCurrentLinkAttrSetModel() override;
  void setLinkType(IKbNode* link_type) override;
  void editFinished() override;

 private:
  SNAIL_DISABLE_COPY(KbNodeLinkAttributePopupEditorModel);

  SNAIL_SIGSLOT_IMPL(LinkTypeChanged);
  SNAIL_SIGSLOT_IMPL(ValidateComplete);

  IKbNodeLinkAttribute* attr_;
  IAttribute* value_attr_copy_;
  IAttributeModelFactory* attr_model_factory_;
};


}  // namespace snailcore

#endif  // KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
