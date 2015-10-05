// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_ATTRIBUTE_COLLECTION_MODEL_FACTORY_H_
#define I_ATTRIBUTE_COLLECTION_MODEL_FACTORY_H_

namespace snailcore {

class IAttributeCollectionModel;
class IAttributeSupplier;

class IAttributeCollectionModelFactory {
 public:
  virtual ~IAttributeCollectionModelFactory() = default;

  virtual std::shared_ptr<IAttributeCollectionModel>
  createAttributeCollectionModel(
      const std::vector<IAttributeSupplier*>& attr_suppliers) = 0;
};

}  // namespace snailcore

#endif  // I_ATTRIBUTE_COLLECTION_MODEL_FACTORY_H_
