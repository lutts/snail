// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_SIMPLE_KBNODE_ADDER_MODEL_FACTORY_H_
#define INCLUDE_CORE_MOCK_SIMPLE_KBNODE_ADDER_MODEL_FACTORY_H_

#include "core/i_simple_kbnode_adder_model_factory.h"

namespace snailcore {
namespace tests {

class MockSimpleKbNodeAdderModelFactory
    : public ISimpleKbNodeAdderModelFactory {
 public:
  MOCK_CONST_METHOD1(
      createSimpleKbNodeAdderModel,
      std::shared_ptr<ISimpleKbNodeAdderModel>(ITreeItemProvider* provider));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_SIMPLE_KBNODE_ADDER_MODEL_FACTORY_H_
