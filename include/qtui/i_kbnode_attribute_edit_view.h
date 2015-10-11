// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_KBNODE_ATTRIBUTE_EDIT_VIEW_H_
#define INCLUDE_QTUI_I_KBNODE_ATTRIBUTE_EDIT_VIEW_H_

#include "qtui/i_attribute_editor_view.h"

class IKbNodeTreeQModel;
class QModelIndex;
class QString;

class IKbNodeAttributeEditView : public IAttributeEditorView {
 public:
  virtual ~IKbNodeAttributeEditView() = default;

  SNAIL_SIGSLOT2(UserClickedIndex, void(const QModelIndex& index));
  SNAIL_SIGSLOT2(FilterPatternChanged, void(const QString& search_str));
  SNAIL_SIGSLOT2(EditingFinished, void(const QString& text));
  SNAIL_SIGSLOT2(UserClickAddKbNode, void());

  virtual void setKbNodeName(const QString& kbnode_name) = 0;
  virtual void setKbNodeTreeQModel(IKbNodeTreeQModel* kbnode_tree_qmodel) = 0;
  virtual void warnMultipleMatch(const QString& provider_name) = 0;
  virtual void warnNotFound(const QString& provider_name) = 0;
  virtual void clearWarningMessages() = 0;
};


#endif  // INCLUDE_QTUI_I_KBNODE_ATTRIBUTE_EDIT_VIEW_H_
