// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/factory/simple_kbnode_adder_model_factory.h"

#include <memory>

#include "src/core/simple_kbnode_adder_model.h"

namespace snailcore {

std::shared_ptr<ISimpleKbNodeAdderModel>
SimpleKbNodeAdderModelFactoryImpl::createSimpleKbNodeAdderModel(
    ITreeItemProvider* provider) const {
  return std::make_shared<SimpleKbNodeAdderModel>(provider, kbnode_manager_);
}

}  // namespace snailcore
