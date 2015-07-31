// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_I_PF_MODEL_H_
#define INCLUDE_PFMVP_I_PF_MODEL_H_

#include "utils/u8string.h"

namespace pfmvp {

class IPfTriadManager;

class IPfModel {
 public:
  using ModelIdType = utils::U8String;

  virtual ~IPfModel() = default;

  virtual const ModelIdType& getModelId() const = 0;
  virtual void onDestroy() { }
  virtual void set_triad_manager(IPfTriadManager*) { }
};

#define DEF_MODEL_ID(id)                                        \
  const ModelIdType& getModelId() const override {              \
    return modelId();                                           \
  }                                                             \
                                                                \
  static const ModelIdType& modelId() {                         \
    static const ModelIdType model_id { #id };                  \
    return model_id;                                            \
  }

/*!
 * there are two reasons to have visitable PfModel,
 *
 *  1. simplify traverse a list of IPfModel derived models
 *  2. some times we drived from IPfModel just because we want many
 *     objects to return in a single list, to simplify the model interface,
 *     but we don't want to have a complete triad for this type of IPfModel,
 *     so we need a method to filter these fake models out of the large list,
 *     visitor pattern is good solution.
 *     e.g. TextModel, displayed as a QLabel, we don't bother to have a triad
 *     for this.
 */

template <typename Visitor>
class VisitableModel : public IPfModel {
 public:
  virtual void accept(Visitor& visitor) = 0;  // NOLINT
};

class IPfModelVisitor {
 public:
  virtual ~IPfModelVisitor() = default;
};

class AcyclicVisitableModel : public IPfModel {
 public:
  virtual void accept(IPfModelVisitor& visitor) = 0;  // NOLINT
  /*
   * typical implementation:
   *
   *  try {
   *    auto& real_visitor = dynamic_cast<XXXVisitor>(visitor);
   *    real_visitor.visit(this);
   *  } catch (std::bad_cast&) { }
   */
};

}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_I_PF_MODEL_H_
