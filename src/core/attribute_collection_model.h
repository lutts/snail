// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_ATTRIBUTE_COLLECTION_MODEL_H_
#define SRC_CORE_ATTRIBUTE_COLLECTION_MODEL_H_

#include <memory>
#include <vector>

#include "utils/basic_utils.h"
#include "snail/i_attribute_collection_model.h"

namespace snailcore {

class IAttributeSupplier;
class IAttributeModelFactory;

class AttributeCollectionModelImpl;

class AttributeCollectionModel : public IAttributeCollectionModel {
 public:
  AttributeCollectionModel(
      const std::vector<IAttributeSupplier*>& attr_supplier_list,
      const IAttributeModelFactory& attr_model_factory);
  virtual ~AttributeCollectionModel();

  bool isEditMode() const override;
  void switchToEditMode() override;
  void switchToDisplayMode() override;

  void traverseAttributes(IAttributeDisplayBlockVisitor* visitor) override;

 private:
  std::unique_ptr<AttributeCollectionModelImpl> impl;
  friend class AttributeCollectionModelImpl;

 private:
  SNAIL_OVERRIDE_SLOT(AttributesChanged);
  SNAIL_OVERRIDE_SLOT(AttrLabelChanged);

 private:
  SNAIL_DISABLE_COPY(AttributeCollectionModel);
};

}  // namespace snailcore

#endif  // SRC_CORE_ATTRIBUTE_COLLECTION_MODEL_H_
