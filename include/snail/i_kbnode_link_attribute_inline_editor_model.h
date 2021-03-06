// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_
#define INCLUDE_SNAIL_I_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"
#include "utils/u8string.h"

namespace snailcore {

class IKbNodeLinkAttributePopupEditorModel;

class IKbNodeLinkAttributeInlineEditorModel : public pfmvp::IPfModel {
 public:
  virtual ~IKbNodeLinkAttributeInlineEditorModel() = default;

  DEF_MODEL_ID(KbNodeLinkAttributeInlineEditorModel);

  virtual utils::U8String valueText() const = 0;

  virtual std::shared_ptr<IKbNodeLinkAttributePopupEditorModel>
  createPopupEditorModel() = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_
