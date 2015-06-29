// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_ATTRIBUTE_ADDER_MODEL_H_
#define SRC_CORE_ATTRIBUTE_ADDER_MODEL_H_

#include <vector>

#include "snail/i_attribute_adder_model.h"
#include "utils/signal_slot_impl.h"
#include "utils/u8string.h"

namespace snailcore {

class IAttributeContainer;
class IAttributeEditorModelFactory;
class IAttributeEditorModel;

class AttributeAdderModel : public IAttributeAdderModel {
 public:
  static const char* prompt_format_str;

  AttributeAdderModel(IAttributeContainer* attr_container,
                      IAttributeEditorModelFactory* attr_model_factory)
      : attr_container_(attr_container)
      , attr_model_factory_(attr_model_factory) { }
  virtual ~AttributeAdderModel();

  utils::U8String getPrompt() const override;
  std::vector<IAttribute*> getAllowedAttributeList() const override;
  int getCurrentAttributeIndex() const override;
  void setCurrentAttributeIndex(int index) override;

  std::shared_ptr<IAttributeEditorModel>
  getCurrentAttributeEditorModel() override;
  bool validateResult() const override;

  void doAddAttribute() override;

 private:
  AttributeAdderModel(const AttributeAdderModel& other) = delete;
  AttributeAdderModel& operator=(const AttributeAdderModel& other) = delete;

  void updateCurrentAttributeEditorModel(int attr_index,
                                         bool initial_create = false);

  IAttributeContainer* attr_container_;
  IAttributeEditorModelFactory* attr_model_factory_;

  std::shared_ptr<IAttributeEditorModel> curr_attr_model_;

  mutable int curr_attr_index_ { -1 };

 private:
  SNAIL_SIGSLOT_IMPL(ValidateComplete)
  SNAIL_SIGSLOT_IMPL(DiscardAttributeEditorModel)
  SNAIL_SIGSLOT_IMPL(CurrentAttributeEditorModelChanged)
};


}  // namespace snailcore

#endif  // SRC_CORE_ATTRIBUTE_ADDER_MODEL_H_
