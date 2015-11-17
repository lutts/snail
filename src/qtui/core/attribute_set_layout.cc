// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/core/attribute_set_layout.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QSignalMapper>
#include <QDebug>

#include <forward_list>
#include <vector>
#include <map>
#include <algorithm>

#include "qsint/include/QSint"
#include "snail/i_attribute_supplier.h"
#include "snail/i_attribute.h"
#include "qtui/ui/i_attribute_editor_view.h"

using snailcore::IAttribute;
using snailcore::IAttributeSupplier;

class AttributeSetLayoutImpl : QObject {
  Q_OBJECT

 public:
  enum {
    kNameColumn = AttributeSetLayout::kNameColumn,
    kValueColumn = AttributeSetLayout::kValueColumn,
  };

  AttributeSetLayoutImpl() : layout_(new QGridLayout()) {
    layout_->setHorizontalSpacing(8);

    connect(layout_, &QObject::destroyed, [this]() { layout_ = nullptr; });

    connect(&deprecated_layout_item_cleanup_timer_, SIGNAL(timeout()),
            &timer_mapper_, SLOT(map()));
    connect(&timer_mapper_, SIGNAL(mapped(int)), this,
            SLOT(cleanupDeprecatedLayoutItems(int)));

    deprecated_layout_item_cleanup_timer_.setSingleShot(true);
  }
  virtual ~AttributeSetLayoutImpl() = default;

  void setAttributeSuppliers(
      const std::vector<snailcore::IAttributeSupplier*>& attr_suppliers,
      bool edit_mode);

  void preRelayout();
  void doEditModeLayout(
      const std::vector<snailcore::IAttributeSupplier*>& attr_suppliers);
  void doDisplayModeLayout(
      const std::vector<snailcore::IAttributeSupplier*>& attr_suppliers);
  void postRelayout(bool to_edit_mode);

  void addAttrRow_editMode(int row, IAttribute* attr);
  void addAttribute(IAttributeSupplier* supplier);

  QLabel* nameLabelOfAttr(IAttribute* attr);

 private slots:
  void cleanupDeprecatedLayoutItems(int to_edit_mode);

 private:
  QLabel* addLabel(const QString& text, int row, int column);
  QLabel* addNameLabel(const utils::U8String& text, int row);

  QGridLayout* layout_;
  QTimer deprecated_layout_item_cleanup_timer_;
  QSignalMapper timer_mapper_;

  std::forward_list<QWidget*> curr_widgets_;
  std::forward_list<QWidget*> deprecated_widgets_;
  std::map<IAttribute*, QLabel*> attr_to_label_map_;
  std::map<IAttributeSupplier*, int> supplier_to_row_map_;

 private:
  SNAIL_DISABLE_COPY(AttributeSetLayoutImpl);

  AttributeSetLayout* q_ptr{nullptr};
  friend class AttributeSetLayout;
};

#include "attribute_set_layout.moc"

void AttributeSetLayoutImpl::preRelayout() {
  deprecated_widgets_ = std::move(curr_widgets_);
  curr_widgets_.clear();
  attr_to_label_map_.clear();
  supplier_to_row_map_.clear();
}

namespace {

bool shouldShowSupplierRow(IAttributeSupplier* supplier) {
  const int max_attrs = supplier->max_attrs();
  const int attr_count = supplier->attr_count();

  return (max_attrs > 1) && (attr_count < max_attrs);
}

bool isSupplierFull(IAttributeSupplier* supplier) {
  const int max_attrs = supplier->max_attrs();
  const int attr_count = supplier->attr_count();

  return attr_count == max_attrs;
}

#if 0
void printWidget(QWidget* widget, const QRect& pos) {
  auto lbl = qobject_cast<QLabel*>(widget);
  if (lbl) {
    qDebug() << "QLabel " << lbl->text() << " @pos: " << pos;
  } else {
    auto albl = qobject_cast<QSint::ActionLabel*>(widget);
    if (albl) {
      qDebug() << "ActionLabel " << albl->text() << " @pos: " << pos;
    } else {
      qDebug() << "attr editor @pos " << pos;
    }
  }
}

void printLayout(QGridLayout* layout) {
  struct ItemData {
    QRect pos;
    QWidget* widget;
  };

  std::vector<ItemData*> item_datas;

  for (int i = 0; i < layout->count(); ++i) {
    int r, c, rs, cs;
    layout->getItemPosition(i, &r, &c, &rs, &cs);
    auto item = layout->itemAt(i);

    item_datas.push_back(new ItemData{QRect(r, c, rs, cs), item->widget()});
  }

  std::sort(item_datas.begin(), item_datas.end(),
            [](const ItemData* a, const ItemData* b) {
              if (a->pos.x() != b->pos.x())
                return a->pos.x() < b->pos.x();
              else
                return a->pos.y() < b->pos.y();
            });

  for (auto & item_data : item_datas) {
    printWidget(item_data->widget, item_data->pos);
  }
}
#endif

void insertRow(QGridLayout* layout, int row) {
  std::map<QLayoutItem*, QRect> item_to_pos;

  for (int idx = 0; idx < layout->count(); ++idx) {
    int r, c, rs, cs;
    layout->getItemPosition(idx, &r, &c, &rs, &cs);
    auto item = layout->itemAt(idx);
    if (r >= row) {
      item_to_pos[item] = QRect(r + 1, c, rs, cs);
    } else {
      item_to_pos[item] = QRect(r, c, rs, cs);
    }
  }

  while (layout->takeAt(0) != 0) {
  }

  for (auto& map_item : item_to_pos) {
    const QRect& r = map_item.second;
    layout->addItem(map_item.first, r.x(), r.y(), r.width(), r.height());
  }
}

void removeLayoutItem(QLayoutItem* item);

void clearLayout(QLayout* layout) {
  QLayoutItem* item;
  while ((item = layout->takeAt(0)) != 0) {
    removeLayoutItem(item);
  }
  layout->invalidate();
}

void removeLayoutItem(QLayoutItem* item) {
  if (item->layout()) {
    clearLayout(item->layout());
    delete item->layout();
  }

  if (item->widget()) {
    item->widget()->deleteLater();
  }
  delete item;
}

void removeRow(QGridLayout* layout, int row) {
  std::map<QLayoutItem*, QRect> item_to_pos;
  std::vector<QLayoutItem*> to_be_removed_items;

  for (int idx = 0; idx < layout->count(); ++idx) {
    int r, c, rs, cs;
    layout->getItemPosition(idx, &r, &c, &rs, &cs);
    auto item = layout->itemAt(idx);
    if (r > row) {
      item_to_pos[item] = QRect(r - 1, c, rs, cs);
    } else if (r < row) {
      item_to_pos[item] = QRect(r, c, rs, cs);
    } else {
      to_be_removed_items.push_back(item);
    }
  }

  while (layout->takeAt(0) != 0) {
  }

  for (auto& map_item : item_to_pos) {
    const QRect& r = map_item.second;
    layout->addItem(map_item.first, r.x(), r.y(), r.width(), r.height());
  }

  for (auto& item : to_be_removed_items) {
    removeLayoutItem(item);
  }
}

}  // namespace

void AttributeSetLayoutImpl::addAttribute(IAttributeSupplier* supplier) {
  if (supplier_to_row_map_.find(supplier) == supplier_to_row_map_.end()) return;

  int row = supplier_to_row_map_[supplier];
  auto new_attr = supplier->addAttribute();
  if (!new_attr) return;

  insertRow(layout_, row);
  addAttrRow_editMode(row, new_attr);

  if (isSupplierFull(supplier)) {
    int supplier_row = row + 1;
    for (int idx = 0; idx < layout_->count(); ++idx) {
      int r, c, rs, cs;
      layout_->getItemPosition(idx, &r, &c, &rs, &cs);
      if (r == supplier_row) {
        QLayoutItem* supp_item = layout_->itemAt(idx);
        curr_widgets_.remove(supp_item->widget());
        break;
      }
    }

    removeRow(layout_, row + 1);
    supplier_to_row_map_.erase(supplier);
  } else {
    supplier_to_row_map_[supplier] = row + 1;
  }
}

void AttributeSetLayoutImpl::addAttrRow_editMode(int row, IAttribute* attr) {
  auto label = addNameLabel(attr->displayName(), row);
  attr_to_label_map_[attr] = label;

  auto ov = q_ptr->CreateAttrEditor(attr);
  if (ov) {
    IAttributeEditorView* attr_editor_view = *ov;
    if (attr_editor_view) {
      layout_->addWidget(attr_editor_view->getWidget(), row, kValueColumn);
      // do NOT add to curr_widgets
    }
  }
}

QLabel* AttributeSetLayoutImpl::nameLabelOfAttr(IAttribute* attr) {
  try {
    return attr_to_label_map_.at(attr);
  } catch (...) {
  }
  return nullptr;
}

void AttributeSetLayoutImpl::doEditModeLayout(
    const std::vector<snailcore::IAttributeSupplier*>& attr_suppliers) {
  int row = 0;
  for (auto& supplier : attr_suppliers) {
    auto attrs = supplier->attributes();
    for (auto& attr : attrs) {
      addAttrRow_editMode(row, attr);
      ++row;
    }

    if (attrs.size() > 0) {
      supplier->whenAttributeChanged(
          [this](IAttribute* attr) {
            QLabel* label = nameLabelOfAttr(attr);
            if (label) label->setText(U8StringToQString(attr->displayName()));
          },
          q_ptr->getSharedPtr());
    }

    if (shouldShowSupplierRow(supplier)) {
      auto supplier_name = U8StringToQString(supplier->name());
      auto add_attr_action_label = new QSint::ActionLabel();
      add_attr_action_label->setText(
          QStringLiteral("Add %1").arg(supplier_name));
      layout_->addWidget(add_attr_action_label, row, 0, 1, 2,
                         Qt::AlignHCenter | Qt::AlignVCenter);

      supplier_to_row_map_[supplier] = row;
      connect(add_attr_action_label, &QSint::ActionLabel::clicked,
              [this, supplier]() { addAttribute(supplier); });
      ++row;
      curr_widgets_.push_front(add_attr_action_label);
    }
  }
}

void AttributeSetLayoutImpl::doDisplayModeLayout(
    const std::vector<snailcore::IAttributeSupplier*>& attr_suppliers) {
  int row = 0;
  for (auto& supplier : attr_suppliers) {
    auto attrs = supplier->attributes();
    std::sort(begin(attrs), end(attrs),
              [](const IAttribute* a, const IAttribute* b)
                  -> bool { return a->displayName() < b->displayName(); });

    utils::U8String last_name;

    for (auto& attr : attrs) {
      auto name = attr->displayName();
      if (name == last_name) {
        name = "";
      } else {
        last_name = name;
      }

      addNameLabel(name, row);
      addLabel(U8StringToQString(attr->valueText()), row, kValueColumn);
      ++row;
    }
  }
}

void AttributeSetLayoutImpl::postRelayout(bool to_edit_mode) {
  auto timer = &deprecated_layout_item_cleanup_timer_;
  int to_mode = to_edit_mode ? 1 : 0;

  timer_mapper_.removeMappings(timer);
  timer_mapper_.setMapping(timer, to_mode);

  timer->start();
}

void AttributeSetLayoutImpl::cleanupDeprecatedLayoutItems(int to_edit_mode) {
  for (auto& widget : deprecated_widgets_) {
    layout_->removeWidget(widget);
    widget->deleteLater();
  }
  deprecated_widgets_.clear();

  if (!to_edit_mode) {
    q_ptr->CloseAttributeEditors();
  }
}

void AttributeSetLayoutImpl::setAttributeSuppliers(
    const std::vector<snailcore::IAttributeSupplier*>& attr_suppliers,
    bool edit_mode) {
  if (!layout_) return;

  preRelayout();
  if (edit_mode) {
    doEditModeLayout(attr_suppliers);
  } else {
    doDisplayModeLayout(attr_suppliers);
  }
  postRelayout(edit_mode);
}

QLabel* AttributeSetLayoutImpl::addLabel(const QString& text, int row,
                                         int column) {
  auto label = new QLabel(text);
  label->setTextInteractionFlags(Qt::LinksAccessibleByMouse |
                                 Qt::TextSelectableByMouse);
  layout_->addWidget(label, row, column);
  curr_widgets_.push_front(label);

  return label;
}

QLabel* AttributeSetLayoutImpl::addNameLabel(const utils::U8String& text,
                                             int row) {
  QString colon_text = U8StringToQString(text);
  if (!colon_text.isEmpty()) colon_text += QStringLiteral(":");
  return addLabel(colon_text, row, kNameColumn);
}

AttributeSetLayout::AttributeSetLayout()
    : impl(utils::make_unique<AttributeSetLayoutImpl>()) {
  impl->q_ptr = this;
}

AttributeSetLayout::~AttributeSetLayout() = default;

void AttributeSetLayout::setAttributeSuppliers(
    const std::vector<snailcore::IAttributeSupplier*>& attr_suppliers,
    bool edit_mode) {
  impl->setAttributeSuppliers(attr_suppliers, edit_mode);
}

QGridLayout* AttributeSetLayout::layout() const { return impl->layout_; }
