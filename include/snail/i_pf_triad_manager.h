// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_PF_TRIAD_MANAGER_H_
#define INCLUDE_SNAIL_I_PF_TRIAD_MANAGER_H_

#include <memory>

#include "snail/i_pf_model.h"
#include "snail/i_pf_view.h"
#include "snail/signal_slot.h"

namespace snailcore {

#define SNAIL_PFTRIAD_SIGSLOT(sigName, ObjType, ...)            \
  using sigName##Signature = __VA_ARGS__;                       \
  using sigName##SlotType = std::function<sigName##Signature>;  \
  virtual void when##sigName(                                   \
      ObjType* obj,                                             \
      sigName##SlotType handler,                                \
      std::shared_ptr<snailcore::ITrackable> trackObject) = 0


#define SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE_(sigName, ObjType)           \
  sigName##SignalType sigName;                                          \
                                                                        \
  void when##sigName(                                                   \
      ObjType* obj,                                                     \
      sigName##SlotType handler,                                        \
      std::shared_ptr<snailcore::ITrackable> trackObject) override

#define SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE(sigName, ObjType)            \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE_(sigName, ObjType)

#define SNAIL_PFTRIAD_SIGSLOT_COMBINER_IMPL_DECLARE(sigName, ObjType, CombT) \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombT>;           \
  SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE_(sigName, ObjType)

class IPfTriadManager {
 public:
  virtual ~IPfTriadManager() = default;

  SNAIL_PFTRIAD_SIGSLOT(RequestRemoveModel,
                        IPfModel, bool(IPfModel* model));  // NOLINT
  SNAIL_PFTRIAD_SIGSLOT(AboutToDestroyModel, IPfModel, void(IPfModel* model));
  SNAIL_PFTRIAD_SIGSLOT(AboutToDestroyView, IPfView, void(IPfView* view));

  virtual std::shared_ptr<IPfView>
  createViewFor(std::shared_ptr<IPfModel> model) = 0;

  virtual void removeTriadBy(IPfModel* model) = 0;
  virtual void removeTriadBy(IPfView* view) = 0;
  virtual bool requestRemoveTriadByView(IPfView* view) = 0;
  virtual IPfView* findViewByModel(IPfModel* model) const = 0;
  virtual IPfModel* findModelByView(IPfView* view) const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_PF_TRIAD_MANAGER_H_
