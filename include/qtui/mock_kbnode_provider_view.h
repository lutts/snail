// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_KBNODE_PROVIDER_VIEW_H_
#define INCLUDE_QTUI_MOCK_KBNODE_PROVIDER_VIEW_H_

#include "qtui/i_kbnode_provider_view.h"

class MockKbNodeProviderView : public IKbNodeProviderView {
 public:
  MOCK_METHOD1(showView, bool(bool));

  SNAIL_MOCK_SLOT(UserSelectIndex);
  SNAIL_MOCK_SLOT(NewKbNodeNameChanged);
  SNAIL_MOCK_SLOT(UserToggleCategoryCheckbox);
  SNAIL_MOCK_SLOT(UserClickAddButton);

  MOCK_METHOD1(setNewKbNodeName, void(const QString& name));
  MOCK_METHOD1(setKbNodeTreeQModel, void(IKbNodeTreeQModel* tree_model));
  MOCK_METHOD1(selectIndex, void(const QModelIndex& index));
  MOCK_METHOD1(setNameValidateResult, void(bool result));
};

#endif  // INCLUDE_QTUI_MOCK_KBNODE_PROVIDER_VIEW_H_
