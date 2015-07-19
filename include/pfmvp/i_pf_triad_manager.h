// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_I_PF_TRIAD_MANAGER_H_
#define INCLUDE_PFMVP_I_PF_TRIAD_MANAGER_H_

#include <memory>
#include <vector>
#include <functional>

#include "utils/i_trackable.h"
#include "pfmvp/i_pf_model.h"
#include "pfmvp/i_pf_view.h"
#include "pfmvp/i_pf_view_factory.h"
#include "pfmvp/i_pf_create_view_args.h"
#include "utils/signal_slot.h"

namespace pfmvp {

#define SNAIL_PFTRIAD_SIGSLOT(sigName, ObjType, ...)            \
  using sigName##Signature = __VA_ARGS__;                       \
  using sigName##SlotType = std::function<sigName##Signature>;  \
  virtual bool when##sigName(                                   \
      ObjType* obj,                                             \
      sigName##SlotType handler,                                \
      std::shared_ptr<utils::ITrackable> trackObject) = 0


#define SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE_(sigName, ObjType)           \
  sigName##SignalType sigName;                                          \
                                                                        \
  bool when##sigName(                                                   \
      ObjType* obj,                                                     \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject) override

#define SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE(sigName, ObjType)            \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE_(sigName, ObjType)

#define SNAIL_PFTRIAD_SIGSLOT_COMBINER_IMPL_DECLARE(sigName, ObjType, CombT) \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombT>;           \
  SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE_(sigName, ObjType)

class IPfTriadManager : public utils::ITrackable {
 public:
  enum {
    kMatchedContinue,
    kMatchedBreak,
    kNotMatched
  };

  virtual ~IPfTriadManager() = default;

  SNAIL_PFTRIAD_SIGSLOT(RequestRemoveModel,
                        IPfModel, bool(IPfModel* model));  // NOLINT
  SNAIL_PFTRIAD_SIGSLOT(AboutToDestroyModel, IPfModel, void(IPfModel* model));
  SNAIL_PFTRIAD_SIGSLOT(AboutToDestroyView, IPfView, void(IPfView* view));

  virtual std::shared_ptr<IPfView>
  createViewFor(std::shared_ptr<IPfModel> model,
                PfPresenter* parent = nullptr,
                bool auto_remove_child = true,
                PfCreateViewArgs* args = nullptr /* IN, OUT */) = 0;

  virtual void removeTriadBy(IPfModel* model) = 0;
  virtual void removeTriadBy(IPfView* view) = 0;
  virtual bool requestRemoveTriadByView(IPfView* view) = 0;

  virtual std::vector<IPfView*> findViewByModel(IPfModel* model) const = 0;
  virtual IPfModel* findModelByView(IPfView* view) const = 0;

  virtual std::vector<IPfView*> findViewByModelAndViewFactory(
      IPfModel* model,
      const IPfViewFactory::ViewFactoryIdType& view_factory_id) const = 0;

  using MementoPredicate =
      std::function<int(const PfCreateViewArgsMemento& memento)>;
  virtual std::vector<IPfView*> findViewByModel_if(
      IPfModel* model,
      MementoPredicate pred) const = 0;
};

}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_I_PF_TRIAD_MANAGER_H_
