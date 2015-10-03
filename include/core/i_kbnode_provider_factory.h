// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_PROVIDER_FACTORY_H_
#define I_KBNODE_PROVIDER_FACTORY_H_

namespace snailcore {

class IKbNodeProvider;
class IKbNodeManager;

class IKbNodeProviderFactory {
 public:
  virtual ~IKbNodeProviderFactory() = default;

  virtual std::shared_ptr<IKbNodeProvider> createKbNodeProvider(
      IKbNode* root_kbnode,
      IKbNodeManager* kbnode_manager) const = 0;
};

}  // namespace snailcore

#endif  // I_KBNODE_PROVIDER_FACTORY_H_
