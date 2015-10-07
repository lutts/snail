// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_ATTRIBUTE_DELEGATE_H_
#define SRC_QTUI_ATTRIBUTE_DELEGATE_H_

#include <QStyledItemDelegate>

#include "qtui/i_attribute_delegate.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

#include <QDebug>

class AttributeDelegate : public IAttributeDelegate
                        , public QStyledItemDelegate {
 public:
  AttributeDelegate() { }
  virtual ~AttributeDelegate() { }

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;

  void destroyEditor(QWidget* editor, const QModelIndex& index) const override;

  void setEditorData(QWidget *editor, const QModelIndex &index) const override {
    (void)editor;
    (void)index;
    // do nothing
  }

  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override {
    (void)editor;
    (void)model;
    (void)index;
    // do nothing
  }

  void updateEditorGeometry(QWidget *editor,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const override {
    qDebug() << "update editor geometry" << option.rect;
    (void)index;
    editor->setGeometry(option.rect);
  }


 private:
  SNAIL_DISABLE_COPY(AttributeDelegate);

 private:
  SNAIL_SIGSLOT_IMPL(CreateEditorFor);
  SNAIL_SIGSLOT_IMPL(CloseEditor);
};


#endif  // SRC_QTUI_ATTRIBUTE_DELEGATE_H_
