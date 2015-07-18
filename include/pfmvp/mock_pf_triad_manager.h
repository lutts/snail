// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_MOCK_PF_TRIAD_MANAGER_H_
#define INCLUDE_PFMVP_MOCK_PF_TRIAD_MANAGER_H_

#include <memory>
#include <vector>
#include "pfmvp/i_pf_triad_manager.h"

namespace pfmvp {
namespace tests {

#define SNAIL_PFTRIAD_MOCK_SLOT(sigName, ObjType)               \
  MOCK_METHOD3(when##sigName, bool(ObjType*, sigName##SlotType, \
                                   std::shared_ptr<utils::ITrackable>))

class MockPfTriadManager : public IPfTriadManager {
 public:
  SNAIL_PFTRIAD_MOCK_SLOT(RequestRemoveModel, IPfModel);
  SNAIL_PFTRIAD_MOCK_SLOT(AboutToDestroyModel, IPfModel);
  SNAIL_PFTRIAD_MOCK_SLOT(AboutToDestroyView, IPfView);

  MOCK_METHOD4(createViewFor,
               std::shared_ptr<IPfView>(std::shared_ptr<IPfModel> model,
                                        PfPresenter* parent,
                                        bool auto_remove_child,
                                        PfCreateViewArgs* args));

  MOCK_METHOD1(removeTriadBy, void(IPfModel* model));
  MOCK_METHOD1(removeTriadBy, void(IPfView* view));
  MOCK_METHOD1(requestRemoveTriadByView, bool(IPfView* view));

  MOCK_CONST_METHOD1(findViewByModel,
                     std::vector<IPfView*>(IPfModel* model));
  MOCK_CONST_METHOD1(findModelByView, IPfModel*(IPfView* view));

  MOCK_CONST_METHOD2(
      findViewByModelAndViewFactory,
      std::vector<IPfView*>(
          IPfModel* model,
          const IPfViewFactory::ViewFactoryIdType& view_factory_id));

  MOCK_CONST_METHOD2(findViewByModel_if,
                     std::vector<IPfView*>(
                         IPfModel* model,
                         MementoPredicate pred));
};

}  // namespace tests
}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_MOCK_PF_TRIAD_MANAGER_H_
