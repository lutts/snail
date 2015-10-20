// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_PROVIDER_FACTORY_H_
#define INCLUDE_CORE_I_KBNODE_PROVIDER_FACTORY_H_

namespace snailcore {

class ITreeItemProvider;
class IKbNodeManager;

class ITreeItemProviderFactory {
 public:
  virtual ~ITreeItemProviderFactory() = default;

  virtual std::shared_ptr<ITreeItemProvider> createTreeItemProvider(
      IKbNode* root_kbnode,
      IKbNodeManager* kbnode_manager) const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_KBNODE_PROVIDER_FACTORY_H_
