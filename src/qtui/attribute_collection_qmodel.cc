// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <memory>
#include <algorithm>
#include <vector>

#include "src/qtui/attribute_collection_qmodel.h"
#include "snail/i_attribute.h"
#include "snail/i_attribute_supplier.h"

using namespace snailcore;  // NOLINT

class AttributeCollectionQModelImpl {
 public:
  void setAttributeSuppliers(std::vector<IAttributeSupplier*> attr_suppliers);
  QString displayText(const QModelIndex& index);

  void switchToEditMode();
  void switchToDisplayMode();

  void mayAddAttributeIfSupplier(int row);

  int rowOfAttr(IAttribute* attr);

 private:
  class AttributeRowData {
   public:
    AttributeRowData(QString name, QString value,
                     IAttributeSupplier* attr_supplier)
        : name_(name), value_(value), attr_supplier_(attr_supplier) { }

    AttributeRowData(QString name, QString value, IAttribute* attr)
        : name_(name), value_(value), attr_(attr) { }

    enum {
      kNameColumn = 0,
      kValueColumn = 1
    };

    QString columnText(int column) {
      switch (column) {
        case kNameColumn:
          return name_;
        case kValueColumn:
          return value_;
        default:
          return QString();
      }
    }

    IAttributeSupplier* attr_supplier() const {
      return attr_supplier_;
    }

    IAttribute* attr() const {
      return attr_;
    }

   private:
    QString name_;
    QString value_;
    IAttribute* attr_ { nullptr };
    IAttributeSupplier* attr_supplier_ { nullptr };
  };

  void fillEmptyMax1SuppliersWithFreshAttributes();
  void clearEmptyAttributes();
  void buildAttrRowData();

  std::vector<IAttributeSupplier*> attr_suppliers_;
  std::vector<AttributeRowData> attr_row_data_;

  bool edit_mode_ { false };

  AttributeCollectionQModel* q_ptr;
  friend class AttributeCollectionQModel;
};

void
AttributeCollectionQModelImpl::fillEmptyMax1SuppliersWithFreshAttributes() {
  for (auto supplier : attr_suppliers_) {
    if ( (supplier->max_attrs() == 1) &&
         (supplier->attr_count() == 0)) {
      supplier->addAttribute();
    }
  }
}

void AttributeCollectionQModelImpl::clearEmptyAttributes() {
  for (auto supplier : attr_suppliers_) {
    auto attrs = supplier->attributes();
    for (auto attr : attrs) {
      if (attr->isEmpty()) {
        supplier->removeAttribute(attr);
      }
    }
  }
}

void AttributeCollectionQModelImpl::buildAttrRowData() {
  attr_row_data_.clear();
  attr_row_data_.reserve(attr_suppliers_.size() * 2);

  if (edit_mode_) {
    fillEmptyMax1SuppliersWithFreshAttributes();
  } else {
    clearEmptyAttributes();
  }

  for (auto supplier : attr_suppliers_) {
    auto attrs = supplier->attributes();

    if (!edit_mode_) {
      std::sort(begin(attrs), end(attrs),
                [](const IAttribute* a, const IAttribute* b) -> bool {
                  return a->displayName() < b->displayName();
                });
    }

    QString last_name { };

    for (auto & attr : attrs) {
      QString name = U8StringToQString(attr->displayName());

      if (!edit_mode_) {
        if (name == last_name)
          name.clear();
        else
          last_name = name;
      }

      QString value = U8StringToQString(attr->valueText());

      attr_row_data_.emplace_back(name, value, attr);
    }

    if (edit_mode_ &&
        (supplier->max_attrs() > 1) &&
        (supplier->attr_count() < supplier->max_attrs())) {
      QString name = U8StringToQString(supplier->name());
      name = QStringLiteral("Add %1").arg(name);
      attr_row_data_.emplace_back(name, QString(), supplier);
    }
  }
}

void AttributeCollectionQModelImpl::setAttributeSuppliers(
    std::vector<IAttributeSupplier*> attr_suppliers) {
  attr_suppliers_ = std::move(attr_suppliers);
  buildAttrRowData();
}

QString AttributeCollectionQModelImpl::displayText(const QModelIndex& index) {
  auto & row_data = attr_row_data_[index.row()];
  return row_data.columnText(index.column());
}

void AttributeCollectionQModelImpl::switchToEditMode() {
  if (edit_mode_)
    return;

  edit_mode_ = true;
  buildAttrRowData();
}

void AttributeCollectionQModelImpl::switchToDisplayMode() {
  if (!edit_mode_)
    return;

  edit_mode_ = false;
  buildAttrRowData();
}

void AttributeCollectionQModelImpl::mayAddAttributeIfSupplier(int row) {
  if (!edit_mode_)
    return;

  auto & row_data = attr_row_data_[row];
  auto attr_supplier = row_data.attr_supplier();
  if (attr_supplier) {
    q_ptr->beginInsertRows(QModelIndex(), row, row);

    auto attr = attr_supplier->addAttribute();

    QString name = U8StringToQString(attr->displayName());
    QString value = U8StringToQString(attr->valueText());

    auto iter = attr_row_data_.begin();
    for (int i = 0; i < row; ++i) {
      ++iter;
    }
    attr_row_data_.insert(iter, {name, value, attr});

    q_ptr->endInsertRows();
    q_ptr->AttributeAdded(row);
  }
}

int AttributeCollectionQModelImpl::rowOfAttr(IAttribute* attr) {
  int row = 0;
  for (auto & row_data : attr_row_data_) {
    if (row_data.attr() == attr) {
      return row;
    }

    ++row;
  }

  return -1;
}

AttributeCollectionQModel::AttributeCollectionQModel()
    : pimpl(utils::make_unique<AttributeCollectionQModelImpl>()) {
  pimpl->q_ptr = this;
}

AttributeCollectionQModel::~AttributeCollectionQModel() = default;

void AttributeCollectionQModel::setAttributeSuppliers(
    std::vector<IAttributeSupplier*> attr_suppliers) {
  pimpl->setAttributeSuppliers(attr_suppliers);

  for (auto & supplier : attr_suppliers) {
    supplier->whenAttributeChanged(
        [this](IAttribute* attr) {
          int row = pimpl->rowOfAttr(attr);
          QModelIndex start_index = index(row, 0);
          QModelIndex end_index = index(row, 1);

          emit dataChanged(start_index, end_index);
        },
        shared_from_this());
  }
}

void AttributeCollectionQModel::aboutToSwitchMode() {
  beginResetModel();
}

void AttributeCollectionQModel::switchToEditMode() {
  pimpl->switchToEditMode();
  endResetModel();
}

void AttributeCollectionQModel::switchToDisplayMode() {
  pimpl->switchToDisplayMode();
  endResetModel();
}

int AttributeCollectionQModel::attrRowCount() const {
  return rowCount();
}

void AttributeCollectionQModel::mayAddAttributeIfSupplier(int row) {
  pimpl->mayAddAttributeIfSupplier(row);
}


QVariant AttributeCollectionQModel::data(
    const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  if (!hasIndex(index.row(), index.column()))
    return QVariant();

  if (role == Qt::DisplayRole) {
    return pimpl->displayText(index);
  }

  return QVariant();
}

Qt::ItemFlags AttributeCollectionQModel::flags(const QModelIndex &index) const {
  auto flags = QAbstractTableModel::flags(index);

  if (pimpl->edit_mode_) {
    flags |= Qt::ItemIsEditable;
  }

  return flags;
}

int AttributeCollectionQModel::rowCount(const QModelIndex &parent) const {
  if (!parent.isValid()) {
    return pimpl->attr_row_data_.size();
  } else {
    return 0;
  }
}

int AttributeCollectionQModel::columnCount(const QModelIndex &) const {
  return 2;
}
