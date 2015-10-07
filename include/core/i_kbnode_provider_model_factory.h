// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_PROVIDER_MODEL_FACTORY_H_
#define INCLUDE_CORE_I_KBNODE_PROVIDER_MODEL_FACTORY_H_

namespace snailcore {

class IKbNodeProvider;
class IKbNodeProviderModel;

class IKbNodeProviderModelFactory {
 public:
  virtual ~IKbNodeProviderModelFactory() = default;

  virtual std::shared_ptr<IKbNodeProviderModel>
  createKbNodeProviderModel(IKbNodeProvider* provider) const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_KBNODE_PROVIDER_MODEL_FACTORY_H_
