// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_
#define INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_

#include "core/factory/i_kbnode_link_attribute_popup_editor_model_factory.h"

namespace snailcore {
namespace tests {

class MockKbNodeLinkAttributePopupEditorModelFactory
    : public IKbNodeLinkAttributePopupEditorModelFactory {
 public:
  MOCK_METHOD1(createPopupEditorModel,
               std::shared_ptr<IKbNodeLinkAttributePopupEditorModel>(
                   fto::KbNodeLinkAttribute* attr));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_
