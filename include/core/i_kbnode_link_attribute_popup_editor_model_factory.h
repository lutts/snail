// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_
#define I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_

#include <memory>

namespace snailcore {

class IKbNodeLinkAttributePopupEditorModel;

class IKbNodeLinkAttributePopupEditorModelFactory {
 public:
  virtual ~IKbNodeLinkAttributePopupEditorModelFactory() = default;

  virtual std::shared_ptr<IKbNodeLinkAttributePopupEditorModel>
  createPopupEditorModel(IKbNodeLinkAttribute* attr) = 0;
};

}  // namespace snailcore

#endif  // I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_
