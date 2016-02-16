// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef WORK_MODEL_FACTORY_H_
#define WORK_MODEL_FACTORY_H_

#include "core/factory/i_work_model_factory.h"

namespace snailcore {

class IAttributeSetModelFactory;

class WorkModelFactoryImpl : public IWorkModelFactory {
 public:
  WorkModelFactoryImpl(IAttributeSetModelFactory* attr_set_model_factory)
      : attr_set_model_factory_{attr_set_model_factory} {}

 private:
  std::shared_ptr<IWorkModel> createWorkModel() override;

  IAttributeSetModelFactory* attr_set_model_factory_;
};

}  // namespace snailcore

#endif  // WORK_MODEL_FACTORY_H_
