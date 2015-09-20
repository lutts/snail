// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_ATTRIBUTE_EDIT_VIEW_H_
#define MOCK_KBNODE_ATTRIBUTE_EDIT_VIEW_H_

#include "qtui/i_kbnode_attribute_edit_view.h"

class MockKbNodeAttributeEditView : public IKbNodeAttributeEditView {
 public:
  SNAIL_MOCK_SLOT(UserClickedIndex);
  SNAIL_MOCK_SLOT(FilterPatternChanged);
  SNAIL_MOCK_SLOT(EditingFinished);
  SNAIL_MOCK_SLOT(UserClickAddKbNode);

  MOCK_METHOD1(setKbNodeName, void(const QString& kbnode_name));
  MOCK_METHOD1(setKbNodeTreeQModel,
               void(IKbNodeTreeQModel* kbnode_tree_qmodel));
  MOCK_METHOD0(warnMultipleMatch, void());
  MOCK_METHOD0(warnNotFound, void());
};

#endif  // MOCK_KBNODE_ATTRIBUTE_EDIT_VIEW_H_
