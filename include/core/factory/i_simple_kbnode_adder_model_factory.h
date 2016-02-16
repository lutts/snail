// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_SIMPLE_KBNODE_ADDER_MODEL_FACTORY_H_
#define INCLUDE_CORE_I_SIMPLE_KBNODE_ADDER_MODEL_FACTORY_H_

#include <memory>

namespace snailcore {

class ITreeItemProvider;
class ISimpleKbNodeAdderModel;

class ISimpleKbNodeAdderModelFactory {
 public:
  virtual ~ISimpleKbNodeAdderModelFactory() = default;

  virtual std::shared_ptr<ISimpleKbNodeAdderModel> createSimpleKbNodeAdderModel(
      ITreeItemProvider* provider) const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_SIMPLE_KBNODE_ADDER_MODEL_FACTORY_H_
