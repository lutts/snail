// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_LINK_ATTRIBUTE_MODEL_H_
#define MOCK_KBNODE_LINK_ATTRIBUTE_MODEL_H_

#include "snail/i_kbnode_link_attribute_inline_editor_model.h"

namespace snailcore {
namespace tests {

class MockKbNodeLinkAttributeInlineEditorModel
    : public IKbNodeLinkAttributeInlineEditorModel {
 public:
  MOCK_CONST_METHOD0(valueText, utils::U8String());

  MOCK_METHOD0(createPopupEditorModel,
               std::shared_ptr<IKbNodeLinkAttributePopupEditorModel>());
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_LINK_ATTRIBUTE_MODEL_H_
