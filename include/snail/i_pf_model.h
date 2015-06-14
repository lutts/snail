// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_PF_MODEL_H_
#define INCLUDE_SNAIL_I_PF_MODEL_H_

#include "src/utils/u8string.h"

namespace snailcore {

class IPfModel {
 public:
  using ModelIdType = utils::U8String;

  virtual ~IPfModel() = default;

  virtual const ModelIdType& getModelId() const = 0;
};

#define DEF_MODEL_ID(id)                                        \
  const ModelIdType& getModelId() const override {        \
    return modelId();                                           \
  }                                                             \
                                                                \
  static const ModelIdType& modelId() {                         \
    static const ModelIdType model_id { #id };                        \
    return model_id;                                            \
  }

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_PF_MODEL_H_
