// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_PROVIDER_MODEL_H_
#define SRC_CORE_KBNODE_PROVIDER_MODEL_H_

#include "snail/i_kbnode_provider_model.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class IKbNodeProvider;
class IKbNodeManager;

class KbNodeProviderModel : public IKbNodeProviderModel {
 public:
  explicit KbNodeProviderModel(IKbNodeProvider* kbnode_provider,
                               IKbNodeManager* kbnode_manager);
  virtual ~KbNodeProviderModel();

  utils::U8String name() const override;
  IKbNodeProvider* getKbNodeProvider() const override;
  utils::U8String getFilterPattern() const override;
  void setFilterPattern(const utils::U8String& filter_pattern) override;
  IKbNode* getNewKbNodeParent() const override;
  void setNewKbNodeParent(IKbNode* kbnode) override;
  void setNewKbNodeName(const utils::U8String& name) override;
  bool isNewKbNodeNameValid() const override;
  void setIsCategory(bool category) override;
  void addKbNode() override;

 private:
  SNAIL_DISABLE_COPY(KbNodeProviderModel);
  SNAIL_SIGSLOT_IMPL(KbNodeAdded);

  IKbNodeProvider* kbnode_provider_ { nullptr };
  IKbNodeManager* kbnode_manager_ { nullptr };
  IKbNode* new_kbnode_parent_ { nullptr };
  utils::U8String new_name_ { "" };
  bool is_category_ { false };
};

}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_PROVIDER_MODEL_H_
