// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_SIMPLE_KBNODE_ADDER_VIEW_H_
#define INCLUDE_QTUI_I_SIMPLE_KBNODE_ADDER_VIEW_H_

#include "qtui/generic_view_base.h"
#include "utils/signal_slot.h"

class QString;
class QModelIndex;
class QAbstractItemModel;

class ISimpleKbNodeAdderView : public GenericViewBase {
 public:
  virtual ~ISimpleKbNodeAdderView() = default;

  SNAIL_SIGSLOT_PURE_VIRTUAL(UserSelectIndex, void(const QModelIndex& index));
  SNAIL_SIGSLOT_PURE_VIRTUAL(NewKbNodeNameChanged, void(const QString& name));
  SNAIL_SIGSLOT_PURE_VIRTUAL(UserToggleCategoryCheckbox, void(bool checked));
  SNAIL_SIGSLOT_PURE_VIRTUAL(UserClickAddButton, void());

  virtual void setProviderName(const QString& provider_name) = 0;
  virtual void setNewKbNodeName(const QString& name) = 0;
  virtual void setKbNodeTreeQModel(QAbstractItemModel* tree_model) = 0;
  virtual void selectIndex(const QModelIndex& index) = 0;
  virtual void setNameValidateResult(bool result) = 0;
};


#endif  // INCLUDE_QTUI_I_SIMPLE_KBNODE_ADDER_VIEW_H_
