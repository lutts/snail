// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_UI_KBNODE_PROVIDER_VIEW_H_
#define SRC_QTUI_UI_KBNODE_PROVIDER_VIEW_H_

#include "qtui/i_kbnode_provider_view.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

class KbNodeProviderViewImpl;

class KbNodeProviderView : public IKbNodeProviderView {
 public:
  KbNodeProviderView();
  virtual ~KbNodeProviderView();

  // IPfView impls
  bool showView(bool modal) override;

  // IKbNodeProviderView
  void setProviderName(const QString& provider_name) override;
  void setNewKbNodeName(const QString& name) override;
  void setKbNodeTreeQModel(IKbNodeTreeQModel* tree_model) override;
  void selectIndex(const QModelIndex& index) override;
  void setNameValidateResult(bool result) override;

 private:
  SNAIL_DISABLE_COPY(KbNodeProviderView);

  SNAIL_SIGSLOT_IMPL(UserSelectIndex);
  SNAIL_SIGSLOT_IMPL(NewKbNodeNameChanged);
  SNAIL_SIGSLOT_IMPL(UserToggleCategoryCheckbox);
  SNAIL_SIGSLOT_IMPL(UserClickAddButton);

  std::unique_ptr<KbNodeProviderViewImpl> impl;
  friend class KbNodeProviderViewImpl;
};


#endif  // SRC_QTUI_UI_KBNODE_PROVIDER_VIEW_H_
