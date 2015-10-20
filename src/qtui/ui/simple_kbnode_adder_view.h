// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_UI_SIMPLE_KBNODE_ADDER_VIEW_H_
#define SRC_QTUI_UI_SIMPLE_KBNODE_ADDER_VIEW_H_

#include "qtui/i_simple_kbnode_adder_view.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

class SimpleKbNodeAdderViewImpl;

class SimpleKbNodeAdderView : public ISimpleKbNodeAdderView {
 public:
  SimpleKbNodeAdderView();
  virtual ~SimpleKbNodeAdderView();

  // IPfView impls
  bool showView(bool modal) override;

  // ISimpleKbNodeAdderView
  void setProviderName(const QString& provider_name) override;
  void setNewKbNodeName(const QString& name) override;
  void setKbNodeTreeQModel(QAbstractItemModel* tree_model) override;
  void selectIndex(const QModelIndex& index) override;
  void setNameValidateResult(bool result) override;

 private:
  SNAIL_DISABLE_COPY(SimpleKbNodeAdderView);

  SNAIL_SIGSLOT_IMPL(UserSelectIndex);
  SNAIL_SIGSLOT_IMPL(NewKbNodeNameChanged);
  SNAIL_SIGSLOT_IMPL(UserToggleCategoryCheckbox);
  SNAIL_SIGSLOT_IMPL(UserClickAddButton);

  std::unique_ptr<SimpleKbNodeAdderViewImpl> impl;
  friend class SimpleKbNodeAdderViewImpl;
};


#endif  // SRC_QTUI_UI_SIMPLE_KBNODE_ADDER_VIEW_H_
