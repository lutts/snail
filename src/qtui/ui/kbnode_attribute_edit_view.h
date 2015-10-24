// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_UI_KBNODE_ATTRIBUTE_EDIT_VIEW_H_
#define SRC_QTUI_UI_KBNODE_ATTRIBUTE_EDIT_VIEW_H_

#include <QWidget>

#include "qtui/ui/i_kbnode_attribute_edit_view.h"

#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

class FilterWidget;
class QLabel;

class KbNodeAttributeEditView : public IKbNodeAttributeEditView
                              , public QWidget {
 public:
  KbNodeAttributeEditView();
  virtual ~KbNodeAttributeEditView();

  QWidget* getWidget() override { return this; }

  void setKbNodeName(const QString& kbnode_name) override;
  void setKbNodeTreeQModel(QAbstractItemModel* kbnode_tree_qmodel) override;
  void warnMultipleMatch(const QString& provider_name) override;
  void warnNotFound(const QString& provider_name) override;
  void clearWarningMessages() override;

 private:
  SNAIL_DISABLE_COPY(KbNodeAttributeEditView);

  SNAIL_SIGSLOT_OVERRIDE(UserClickedIndex);
  SNAIL_SIGSLOT_OVERRIDE(FilterPatternChanged);
  SNAIL_SIGSLOT_OVERRIDE(EditingFinished);
  SNAIL_SIGSLOT_OVERRIDE(UserClickAddKbNode);

  FilterWidget* filter_widget_;
  QLabel* err_msg_label_;
};


#endif  // SRC_QTUI_UI_KBNODE_ATTRIBUTE_EDIT_VIEW_H_
