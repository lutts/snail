// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_FEW_ENTITY_ATTR_EDITOR_VIEW_H_
#define MOCK_FEW_ENTITY_ATTR_EDITOR_VIEW_H_

#include "qtui/i_few_entity_attr_editor_view.h"

class MockFewEntityAttrEditorView : public IFewEntityAttrEditorView {
 public:
  SNAIL_MOCK_SLOT(EntitySelectionChanged);

  MOCK_METHOD1(setEntitySelectorQModel,
               void(ICandidateItemQModelAdapter* model));
  MOCK_METHOD1(setCurrentEntityName,
               void(const utils::U8String& entity_name));
};

#endif  // MOCK_FEW_ENTITY_ATTR_EDITOR_VIEW_H_
