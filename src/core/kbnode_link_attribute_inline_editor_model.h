// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_
#define KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_

#include "snail/i_kbnode_link_attribute_inline_editor_model.h"
#include "utils/basic_utils.h"

namespace snailcore {

class IKbNodeLinkAttribute;
class IKbNodeLinkAttributePopupEditorModelFactory;

class KbNodeLinkAttributeInlineEditorModel
    : public IKbNodeLinkAttributeInlineEditorModel {
 public:
  KbNodeLinkAttributeInlineEditorModel(
      IKbNodeLinkAttribute* attr,
      IKbNodeLinkAttributePopupEditorModelFactory* popup_editor_factory);
  virtual ~KbNodeLinkAttributeInlineEditorModel();

  utils::U8String valueText() const override;

  std::shared_ptr<IKbNodeLinkAttributePopupEditorModel>
  createPopupEditorModel() override;

 private:
  SNAIL_DISABLE_COPY(KbNodeLinkAttributeInlineEditorModel);

  IKbNodeLinkAttribute* attr_;
  IKbNodeLinkAttributePopupEditorModelFactory* popup_editor_factory_;
};


}  // namespace snailcore

#endif  // KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_
