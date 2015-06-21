// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_PFMVP_PF_TRIAD_MANAGER_H_
#define SRC_PFMVP_PF_TRIAD_MANAGER_H_

#include <vector>
#include <memory>  // std::unique_ptr

#include "pfmvp/i_pf_triad_manager.h"
#include "pfmvp/i_pf_view_factory.h"
#include "utils/signal_slot_impl.h"
#include "utils/and_slot_bool_result_combiner.h"

namespace pfmvp {

class PfTriadManagerImpl;

class PfTriadManager : public IPfTriadManager {
 public:
  explicit PfTriadManager(const IPfViewFactoryManager& view_factory_mgr);
  virtual ~PfTriadManager();

  std::shared_ptr<IPfView>
  createViewFor(std::shared_ptr<IPfModel> model,
                PfPresenter* parent_presenter,
                bool auto_remove_child) override;

  std::shared_ptr<IPfView>
  createViewFor(
      std::shared_ptr<IPfModel> model,
      const IPfViewFactory::ViewFactoryIdType& view_factory_id,
      PfPresenter* parent_presenter,
      bool auto_remove_child) override;

  void removeTriadBy(IPfModel* model) override;
  void removeTriadBy(IPfView* view) override;
  bool requestRemoveTriadByView(IPfView* view) override;
  std::vector<IPfView*> findViewByModel(IPfModel* model) const override;
  std::vector<IPfView*>
  findViewsByModelId(const IPfModel::ModelIdType& model_id) const override;
  IPfModel* findModelByView(IPfView* view) const override;

 private:
  PfTriadManager(const PfTriadManager& other) = delete;
  PfTriadManager& operator=(const PfTriadManager& other) = delete;

  friend class PfTriadManagerImpl;
  std::unique_ptr<PfTriadManagerImpl> impl;

 private:
  SNAIL_PFTRIAD_SIGSLOT_COMBINER_IMPL_DECLARE(RequestRemoveModel,
                                              IPfModel,
                                              and_slot_bool_result_combiner);
  SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE(AboutToDestroyModel, IPfModel);
  SNAIL_PFTRIAD_SIGSLOT_IMPL_DECLARE(AboutToDestroyView, IPfView);
};


}  // namespace pfmvp

#endif  // SRC_PFMVP_PF_TRIAD_MANAGER_H_
