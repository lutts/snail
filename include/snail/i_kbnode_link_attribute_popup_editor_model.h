// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
#define I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IKbNodeLinkAttributePopupEditorModel : public pfmvp::IPfModel {
 public:
  virtual ~IKbNodeLinkAttributePopupEditorModel() = default;

  DEF_MODEL_ID(KbNodeLinkAttributePopupEditorModel);
};

}  // namespace snailcore

#endif  // I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
