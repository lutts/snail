// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_UI_I_KBNODE_ATTRIBUTE_EDIT_VIEW_H_
#define INCLUDE_QTUI_UI_I_KBNODE_ATTRIBUTE_EDIT_VIEW_H_

#include "qtui/ui/i_attribute_editor_view.h"

class QModelIndex;
class QAbstractItemModel;
class QString;

class IKbNodeAttributeEditView : public IAttributeEditorView {
 public:
  virtual ~IKbNodeAttributeEditView() = default;

  SNAIL_SIGSLOT_PURE_VIRTUAL(UserClickedIndex, void(const QModelIndex& index));
  SNAIL_SIGSLOT_PURE_VIRTUAL(FilterPatternChanged,
                             void(const QString& search_str));
  SNAIL_SIGSLOT_PURE_VIRTUAL(EditingFinished, void(const QString& text));
  SNAIL_SIGSLOT_PURE_VIRTUAL(UserClickAddKbNode, void());

  virtual void setKbNodeName(const QString& kbnode_name) = 0;
  virtual void setKbNodeTreeQModel(QAbstractItemModel* kbnode_tree_qmodel) = 0;
  virtual void warnMultipleMatch(const QString& provider_name) = 0;
  virtual void warnNotFound(const QString& provider_name) = 0;
  virtual void clearWarningMessages() = 0;
};

#endif  // INCLUDE_QTUI_UI_I_KBNODE_ATTRIBUTE_EDIT_VIEW_H_
