// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_UI_KBNODE_ATTRIBUTE_EDIT_VIEW_H_
#define SRC_QTUI_UI_KBNODE_ATTRIBUTE_EDIT_VIEW_H_

#include <QWidget>

#include "qtui/i_kbnode_attribute_edit_view.h"

#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

class QtLineEdit;
class QLabel;

class KbNodeAttributeEditView : public IKbNodeAttributeEditView
                              , public QWidget {
 public:
  KbNodeAttributeEditView();
  virtual ~KbNodeAttributeEditView();

  QWidget* getWidget() override { return this; }

  void setKbNodeName(const QString& kbnode_name) override;
  void setKbNodeTreeQModel(IKbNodeTreeQModel* kbnode_tree_qmodel) override;
  void warnMultipleMatch() override;
  void warnNotFound() override;

 private:
  SNAIL_DISABLE_COPY(KbNodeAttributeEditView);

  SNAIL_SIGSLOT_IMPL(UserClickedIndex);
  SNAIL_SIGSLOT_IMPL(FilterPatternChanged);
  SNAIL_SIGSLOT_IMPL(EditingFinished);
  SNAIL_SIGSLOT_IMPL(UserClickAddKbNode);

  QtLineEdit* line_edit_;
  QLabel* err_msg_label_;
};


#endif  // SRC_QTUI_UI_KBNODE_ATTRIBUTE_EDIT_VIEW_H_
