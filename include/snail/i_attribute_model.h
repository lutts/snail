// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ATTRIBUTE_MODEL_H_
#define INCLUDE_SNAIL_I_ATTRIBUTE_MODEL_H_

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"
#include "utils/u8string.h"

namespace snailcore {

class IAttributeModel : public pfmvp::IPfModel {
 public:
  virtual ~IAttributeModel() = default;

  DEF_MODEL_ID(AttributeModel);

  virtual utils::U8String displayName() const = 0;

  int row() const { return row_; }
  void set_row(int row) { row_ = row; }

  int column() const { return column_; }
  void set_column(int column) { column_ = column; }

 private:
  int row_;
  int column_;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ATTRIBUTE_MODEL_H_
