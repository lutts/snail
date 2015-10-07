// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/core/attribute_delegate.h"
#include "qtui/i_attribute_editor_view.h"

QWidget * AttributeDelegate::createEditor(
    QWidget *parent, const QStyleOptionViewItem &option,
    const QModelIndex &index) const {
  (void)parent;
  (void)option;
  IAttributeEditorView* editor = *CreateEditorFor(index.row());
  if (editor) {
    auto widget = editor->getWidget();
    widget->setParent(parent);
    return widget;
  } else {
    return nullptr;
  }
}

void AttributeDelegate::destroyEditor(
    QWidget* editor, const QModelIndex& index) const {
  (void)index;
  auto editor_view = dynamic_cast<IAttributeEditorView*>(editor);
  CloseEditor(editor_view);
}
