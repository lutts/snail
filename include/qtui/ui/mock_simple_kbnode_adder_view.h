// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_UI_MOCK_SIMPLE_KBNODE_ADDER_VIEW_H_
#define INCLUDE_QTUI_UI_MOCK_SIMPLE_KBNODE_ADDER_VIEW_H_

#include "qtui/ui/i_simple_kbnode_adder_view.h"

class MockSimpleKbNodeAdderView : public ISimpleKbNodeAdderView {
 public:
  MOCK_METHOD1(showView, bool(bool));

  SNAIL_MOCK_SLOT(UserSelectIndex);
  SNAIL_MOCK_SLOT(NewKbNodeNameChanged);
  SNAIL_MOCK_SLOT(UserToggleCategoryCheckbox);
  SNAIL_MOCK_SLOT(UserClickAddButton);

  MOCK_METHOD1(setProviderName, void(const QString& provider_name));
  MOCK_METHOD1(setNewKbNodeName, void(const QString& name));
  MOCK_METHOD1(setKbNodeTreeQModel, void(QAbstractItemModel* tree_model));
  MOCK_METHOD1(selectIndex, void(const QModelIndex& index));
  MOCK_METHOD1(setNameValidateResult, void(bool result));
};

#endif  // INCLUDE_QTUI_UI_MOCK_SIMPLE_KBNODE_ADDER_VIEW_H_
