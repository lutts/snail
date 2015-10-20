// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
#define MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_

#include "snail/i_kbnode_link_attribute_popup_editor_model.h"

namespace snailcore {
namespace tests {

class MockKbNodeLinkAttributePopupEditorModel
    : public IKbNodeLinkAttributePopupEditorModel {
 public:
  SNAIL_MOCK_SLOT(LinkTypeChanged);
  SNAIL_MOCK_SLOT(ValidateComplete);

  MOCK_METHOD0(createValueKbNodeAttrModel, std::shared_ptr<IAttributeModel>());
  MOCK_CONST_METHOD0(getLinkTypeProvider, ITreeItemProvider*());
  MOCK_CONST_METHOD0(getCurrentLinkType, IKbNode*());
  MOCK_METHOD0(getCurrentLinkAttrSetModel,
               std::shared_ptr<IAttributeSetModel>());
  MOCK_METHOD1(setLinkType, void(IKbNode* link_type));
  MOCK_METHOD0(editFinished, void());
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_MODEL_H_
