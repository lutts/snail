// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ATTRIBUTE_COLLECTION_MODEL_H_
#define INCLUDE_SNAIL_I_ATTRIBUTE_COLLECTION_MODEL_H_

#include <vector>

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IAttributeSupplier;
class IAttribute;
class IAttributeModel;

class IAttributeCollectionModel : public pfmvp::IPfModel {
 public:
  virtual ~IAttributeCollectionModel() = default;

  DEF_MODEL_ID(AttributeCollectionModel);

  virtual std::vector<IAttributeSupplier*> getAttributeSuppliers() const = 0;
  virtual
  std::shared_ptr<IAttributeModel> createAttributeModel(IAttribute* attr) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ATTRIBUTE_COLLECTION_MODEL_H_
