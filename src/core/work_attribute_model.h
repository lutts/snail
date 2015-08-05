// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_WORK_ATTRIBUTE_MODEL_H_
#define SRC_CORE_WORK_ATTRIBUTE_MODEL_H_

#include <memory>

#include "utils/basic_utils.h"
#include "snail/i_work_attribute_model.h"

namespace snailcore {

class IAttributeSupplier;
class IAttributeModelFactory;

class WorkAttributeModelImpl;

class WorkAttributeModel
    : public IWorkAttributeModel
    , public std::enable_shared_from_this<WorkAttributeModel> {
 public:
  WorkAttributeModel(const std::vector<IAttributeSupplier*>& attr_supplier_list,
                     const IAttributeModelFactory& attr_model_factory);
  virtual ~WorkAttributeModel();

  bool isEditMode() const override;
  void switchToEditMode() override;
  void switchToDisplayMode() override;

  void traverseAttributes(IAttributeDisplayBlockVisitor* visitor) override;

 private:
  std::unique_ptr<WorkAttributeModelImpl> impl;
  friend class WorkAttributeModelImpl;

 private:
  SNAIL_OVERRIDE_SLOT(AttributesChanged);
  SNAIL_OVERRIDE_SLOT(AttrLabelChanged);

 private:
  SNAIL_DISABLE_COPY(WorkAttributeModel);
};

}  // namespace snailcore

#endif  // SRC_CORE_WORK_ATTRIBUTE_MODEL_H_
