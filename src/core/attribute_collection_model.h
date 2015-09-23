// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_ATTRIBUTE_COLLECTION_MODEL_H_
#define SRC_CORE_ATTRIBUTE_COLLECTION_MODEL_H_

#include <vector>

#include "utils/basic_utils.h"
#include "snail/i_attribute_collection_model.h"

namespace snailcore {

class IAttributeModelFactory;

class AttributeCollectionModel : public IAttributeCollectionModel {
 public:
  AttributeCollectionModel(
      const std::vector<IAttributeSupplier*>& attr_supplier_list,
      const IAttributeModelFactory& attr_model_factory);
  virtual ~AttributeCollectionModel();

  std::vector<IAttributeSupplier*> getAttributeSuppliers() const override;

  std::shared_ptr<IAttributeModel>
  createAttributeModel(IAttribute* attr) override;

 private:
  std::vector<IAttributeSupplier*> attr_suppliers_;
  const IAttributeModelFactory& attr_model_factory_;

 private:
  SNAIL_DISABLE_COPY(AttributeCollectionModel)
};


}  // namespace snailcore

#endif  // SRC_CORE_ATTRIBUTE_COLLECTION_MODEL_H_