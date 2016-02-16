// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SIMPLE_KBNODE_ADDER_MODEL_FACTORY_H_
#define SIMPLE_KBNODE_ADDER_MODEL_FACTORY_H_

#include "include/config.h"
#include "core/factory/i_simple_kbnode_adder_model_factory.h"

namespace snailcore {

FTO_BEGIN_NAMESPACE
class KbNodeManager;
FTO_END_NAMESPACE

class SimpleKbNodeAdderModelFactoryImpl
    : public ISimpleKbNodeAdderModelFactory {
 public:
  SimpleKbNodeAdderModelFactoryImpl(fto::KbNodeManager* kbnode_manager)
      : kbnode_manager_{kbnode_manager} {}

 private:
  std::shared_ptr<ISimpleKbNodeAdderModel> createSimpleKbNodeAdderModel(
      ITreeItemProvider* provider) const override;

  fto::KbNodeManager* kbnode_manager_;
};

}  // namespace snailcore

#endif  // SIMPLE_KBNODE_ADDER_MODEL_FACTORY_H_
