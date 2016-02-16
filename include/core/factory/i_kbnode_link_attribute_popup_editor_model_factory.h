// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_
#define INCLUDE_CORE_I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_

#include <memory>

namespace snailcore {

class IKbNodeLinkAttributePopupEditorModel;

class IKbNodeLinkAttributePopupEditorModelFactory {
 public:
  virtual ~IKbNodeLinkAttributePopupEditorModelFactory() = default;

  virtual std::shared_ptr<IKbNodeLinkAttributePopupEditorModel>
  createPopupEditorModel(fto::KbNodeLinkAttribute* attr) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_
