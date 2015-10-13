// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_
#define MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_

#include "core/i_kbnode_link_attribute_popup_editor_model_factory.h"

namespace snailcore {
namespace tests {

class MockKbNodeLinkAttributePopupEditorModelFactory
    : public IKbNodeLinkAttributePopupEditorModelFactory {
 public:
  MOCK_METHOD1(createPopupEditorModel,
               std::shared_ptr<IKbNodeLinkAttributePopupEditorModel>(
                   IKbNodeLinkAttribute* attr));
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_FACTORY_H_
