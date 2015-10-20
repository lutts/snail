// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_
#define INCLUDE_SNAIL_MOCK_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_

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

#endif  // INCLUDE_SNAIL_MOCK_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_MODEL_H_
