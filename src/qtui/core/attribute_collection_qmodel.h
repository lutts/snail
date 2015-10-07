// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CORE_ATTRIBUTE_COLLECTION_QMODEL_H_
#define SRC_QTUI_CORE_ATTRIBUTE_COLLECTION_QMODEL_H_

#include <QAbstractTableModel>

#include <vector>

#include "qtui/i_attribute_collection_qmodel.h"

#include "utils/basic_utils.h"
#include "utils/i_trackable.h"
#include "utils/signal_slot_impl.h"

class AttributeCollectionQModelImpl;

class AttributeCollectionQModel
    : public IAttributeCollectionQModel
    , public QAbstractTableModel
    , public utils::ITrackable
    , public std::enable_shared_from_this<AttributeCollectionQModel>{
 public:
  AttributeCollectionQModel();
  virtual ~AttributeCollectionQModel();

  // IAttributeCollectionQModel
  void setAttributeSuppliers(
      std::vector<IAttributeSupplier*> attr_suppliers) override;
  void switchToEditMode() override;
  void switchToDisplayMode() override;
  int attrRowCount() const override;
  void mayAddAttributeIfSupplier(int row) override;
  IAttribute* attrOfRow(int row) const override;

  // QAbstractTableModel
  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

 private:
  SNAIL_DISABLE_COPY(AttributeCollectionQModel);

  std::unique_ptr<AttributeCollectionQModelImpl> pimpl;
  friend class AttributeCollectionQModelImpl;

 private:
  SNAIL_SIGSLOT_IMPL(AttributeAdded);
};


#endif  // SRC_QTUI_CORE_ATTRIBUTE_COLLECTION_QMODEL_H_
