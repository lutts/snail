// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ATTRIBUTE_EDITOR_MODEL_FACTORY_H_
#define INCLUDE_SNAIL_I_ATTRIBUTE_EDITOR_MODEL_FACTORY_H_

namespace snailcore {

class IAttributeEditorModelFactory {
 public:
  virtual ~IAttributeEditorModelFactory() = default;

  virtual std::shared_ptr<IAttributeEditorModel>
  createAttributeEditorModel(IAttribute* attr) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ATTRIBUTE_EDITOR_MODEL_FACTORY_H_
