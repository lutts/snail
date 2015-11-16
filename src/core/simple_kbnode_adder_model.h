// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_SIMPLE_KBNODE_ADDER_MODEL_H_
#define SRC_CORE_SIMPLE_KBNODE_ADDER_MODEL_H_

#include "include/config.h"
#include "snail/i_simple_kbnode_adder_model.h"
#include "utils/basic_utils.h"

namespace snailcore {

class IKbNode;
class ITreeItemProvider;

FTO_BEGIN_NAMESPACE
class KbNodeManager;
FTO_END_NAMESPACE

class SimpleKbNodeAdderModel : public ISimpleKbNodeAdderModel {
 public:
  explicit SimpleKbNodeAdderModel(ITreeItemProvider* kbnode_provider,
                               fto::KbNodeManager* kbnode_manager);
  virtual ~SimpleKbNodeAdderModel();

  utils::U8String name() const override;
  ITreeItemProvider* getKbNodeProvider() const override;
  utils::U8String getDefaultNewKbNodeName() const override;
  ITreeItem* getNewKbNodeParent() const override;
  void setNewKbNodeParent(ITreeItem* kbnode) override;
  void setNewKbNodeName(const utils::U8String& name) override;
  bool isNewKbNodeNameValid() const override;
  void setIsCategory(bool category) override;
  void addKbNode() override;

 private:
  SNAIL_DISABLE_COPY(SimpleKbNodeAdderModel);

  ITreeItemProvider* kbnode_provider_ { nullptr };
  fto::KbNodeManager* kbnode_manager_ { nullptr };
  IKbNode* new_kbnode_parent_ { nullptr };
  utils::U8String new_name_ { "" };
  bool is_category_ { false };
};

}  // namespace snailcore

#endif  // SRC_CORE_SIMPLE_KBNODE_ADDER_MODEL_H_
