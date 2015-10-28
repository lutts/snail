// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
#define SRC_CORE_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_

#include "include/config.h"
#include "snail/i_kbnode_link_attribute_popup_editor_model.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"
#include "src/core/kbnode_attribute.h"
#include "src/core/link_type.h"

namespace snailcore {

FTO_BEGIN_NAMESPACE
class KbNodeLinkAttribute;
FTO_END_NAMESPACE

class IAttribute;
class IAttributeModelFactory;
class IAttributeSetModelFactory;

class KbNodeLinkAttributePopupEditorModel
    : public IKbNodeLinkAttributePopupEditorModel {
 public:
  KbNodeLinkAttributePopupEditorModel(
      fto::KbNodeLinkAttribute* attr,
      IAttributeModelFactory* attr_model_factory,
      IAttributeSetModelFactory* attr_set_model_factory);
  virtual ~KbNodeLinkAttributePopupEditorModel();

  std::shared_ptr<IAttributeModel> createValueAttrModel() override;
  ITreeItemProvider* getLinkTypeItemProvider() const override;
  ITreeItem* getCurrentLinkType() const override;
  std::shared_ptr<IAttributeSetModel> getCurrentLinkAttrSetModel() override;
  void setLinkType(ITreeItem* link_type_item) override;
  void editFinished() override;

  KbNodeAttribute* getCurrentValueAttr();

 private:
  SNAIL_DISABLE_COPY(KbNodeLinkAttributePopupEditorModel);

  SNAIL_SIGSLOT_OVERRIDE(LinkTypeChanged);
  SNAIL_SIGSLOT_OVERRIDE(ValidateComplete);

  void initLocalValueAttrCopy();

  void mayInitCurrLinkType();

  fto::KbNodeLinkAttribute* attr_;
  IAttributeModelFactory* attr_model_factory_;
  IAttributeSetModelFactory* attr_set_model_factory_;

  KbNodeAttribute value_attr_copy_;
  bool value_attr_copy_initialized_ { false };

  mutable LinkType curr_link_type_;
  mutable bool curr_link_type_initialized_ { false };
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
