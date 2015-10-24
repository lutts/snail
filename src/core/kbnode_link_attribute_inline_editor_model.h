// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_
#define SRC_CORE_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_

#include "include/config.h"
#include "snail/i_kbnode_link_attribute_inline_editor_model.h"
#include "utils/basic_utils.h"

namespace snailcore {

FTO_BEGIN_NAMESPACE
class KbNodeLinkAttribute;
FTO_END_NAMESPACE

class IKbNodeLinkAttributePopupEditorModelFactory;

class KbNodeLinkAttributeInlineEditorModel
    : public IKbNodeLinkAttributeInlineEditorModel {
 public:
  KbNodeLinkAttributeInlineEditorModel(
      fto::KbNodeLinkAttribute* attr,
      IKbNodeLinkAttributePopupEditorModelFactory* popup_editor_factory);
  virtual ~KbNodeLinkAttributeInlineEditorModel();

  utils::U8String valueText() const override;

  std::shared_ptr<IKbNodeLinkAttributePopupEditorModel>
  createPopupEditorModel() override;

 private:
  SNAIL_DISABLE_COPY(KbNodeLinkAttributeInlineEditorModel);

  fto::KbNodeLinkAttribute* attr_;
  IKbNodeLinkAttributePopupEditorModelFactory* popup_editor_factory_;
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_
