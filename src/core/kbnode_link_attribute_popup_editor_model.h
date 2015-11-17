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
#include "core/fto_kbnode_attribute.h"
#include "core/fto_link_type.h"

namespace snailcore {

FTO_BEGIN_NAMESPACE
class KbNodeLinkAttribute;
class LinkType;
FTO_END_NAMESPACE

class IAttributeModelFactory;
class IAttributeSetModelFactory;

class KbNodeLinkAttributePopupEditorModelSignalHelper;

class KbNodeLinkAttributePopupEditorModel
    : public IKbNodeLinkAttributePopupEditorModel {
 public:
  KbNodeLinkAttributePopupEditorModel(
      fto::KbNodeLinkAttribute* attr,
      IAttributeModelFactory* attr_model_factory,
      IAttributeSetModelFactory* attr_set_model_factory);
  virtual ~KbNodeLinkAttributePopupEditorModel();

  utils::U8String valueAttrName() const override;
  std::shared_ptr<IAttributeModel> createValueAttrModel() override;
  ITreeItemProvider* getLinkTypeItemProvider() const override;
  const ITreeItem* getCurrentProtoLinkType() const override;
  std::shared_ptr<IAttributeSetModel> getCurrentLinkAttrSetModel() override;
  void setProtoLinkType(ITreeItem* proto_link_type_item) override;
  void editFinished() override;

 public:
  SNAIL_SIGSLOT_OVERRIDE(LinkTypeChanged);
  SNAIL_SIGSLOT_OVERRIDE(ValidateComplete);

 private:
  SNAIL_DISABLE_COPY(KbNodeLinkAttributePopupEditorModel);
  void validateComplete();

  std::unique_ptr<KbNodeLinkAttributePopupEditorModelSignalHelper>
      signal_helper_;

  fto::KbNodeLinkAttribute* attr_;
  TEST_PROXY(KbNodeAttribute) value_attr_copy_;
  TEST_PROXY(LinkType) link_type_copy_;

  IAttributeModelFactory* attr_model_factory_;
  IAttributeSetModelFactory* attr_set_model_factory_;

  IAttributeSetModel* curr_attr_set_model_;
  bool attr_model_valid_{true};
  bool attr_set_model_valid_{true};
};

}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
