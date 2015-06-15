// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_PF_TRIAD_MANAGER_H_
#define SRC_CORE_PF_TRIAD_MANAGER_H_

#include <memory>  // std::unique_ptr

#include "snail/i_pf_triad_manager.h"
#include "snail/signal_slot_impl.h"
#include "snail/and_slot_bool_result_combiner.h"

namespace snailcore {

class PfTriadManager : public IPfTriadManager {
 public:
  PfTriadManager();
  virtual ~PfTriadManager();

  std::shared_ptr<IPfView>
  createViewFor(std::shared_ptr<IPfModel> model) override;

  void removeTriadBy(IPfModel* model) override;
  void removeTriadBy(IPfView* view) override;
  bool requestRemoveTriadByView(IPfView* view) override;
  IPfView* findViewByModel(IPfModel* model) const override;
  IPfModel* findModelByView(IPfView* view) const override;

 private:
  PfTriadManager(const PfTriadManager& other) = delete;
  PfTriadManager& operator=(const PfTriadManager& other) = delete;

  void emitAboutToDestroySignal(IPfModel* model, IPfView* view);

  class PfTriadManagerImpl;
  std::unique_ptr<PfTriadManagerImpl> impl;

 private:
  SNAIL_PFTRIAD_SIGSLOT_COMBINER_IMPL_DECLARE(RequestRemoveModel,
                                              IPfModel,
                                              and_slot_bool_result_combiner);
  SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE(AboutToDestroyModel, IPfModel);
  SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE(AboutToDestroyView, IPfView);
};


}  // namespace snailcore

#endif  // SRC_CORE_PF_TRIAD_MANAGER_H_
