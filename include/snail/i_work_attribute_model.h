// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_WORK_ATTRIBUTE_MODEL_H_
#define I_WORK_ATTRIBUTE_MODEL_H_

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

namespace utils {
class Command;
}  // namespace utils

namespace snailcore {

class IAttributeModel;
class ICommandModel;

class IWorkAttributeModel : public pfmvp::IPfModel {
 public:
  class Location {
   public:
    Location(int row, int column,
             int row_span = 1, int column_span = 1)
        : row_(row)
        , column_(column)
        , row_span_(row_span)
        , column_span_(column_span) { }

    int row() const { return row_; }
    int column() const { return column_; }
    int row_span() const { return row_span_; }
    int column_span() const { return column_span_; }

   private:
    int row_;
    int column_;
    int row_span_;
    int column_span_;
  };

  virtual ~IWorkAttributeModel() = default;

  DEF_MODEL_ID(WorkAttributeModel);

  virtual bool isEditMode() const = 0;
  virtual void switchToEditMode() = 0;
  virtual void switchToDisplayMode() = 0;

  SNAIL_SIGSLOT2(AttributesChanged, void());
  SNAIL_SIGSLOT2(ShowPopupFor,
                 bool(std::shared_ptr<IAttributeModel> attr_model));

  virtual
  std::vector<std::shared_ptr<IAttributeModel>> getAttributeModels() const = 0;

  virtual
  std::vector<utils::Command*> getEraseCommands() const = 0;
  virtual
  std::vector<utils::Command*> getPopupEditorCommands() const = 0;
  virtual
  std::vector<utils::Command*> getAddAttributeCommands() const = 0;

  virtual Location getLocation(IPfModel* model) const = 0;
  virtual Location getLocation(utils::Command* command) const = 0;
};

}  // namespace snailcore

#endif  // I_WORK_ATTRIBUTE_MODEL_H_
