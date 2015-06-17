// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_MOCK_PF_TRIAD_MANAGER_H_
#define INCLUDE_PFMVP_MOCK_PF_TRIAD_MANAGER_H_

#include <vector>
#include "pfmvp/i_pf_triad_manager.h"

namespace pfmvp {
namespace tests {

#define SNAIL_PFTRIAD_MOCK_SLOT(sigName, ObjType)               \
  MOCK_METHOD3(when##sigName,                                   \
               bool(ObjType*, sigName##SlotType,                \
                    std::shared_ptr<utils::ITrackable>))

class MockPfTriadManager : public IPfTriadManager {
 public:
  SNAIL_PFTRIAD_MOCK_SLOT(RequestRemoveModel, IPfModel);
  SNAIL_PFTRIAD_MOCK_SLOT(AboutToDestroyModel, IPfModel);
  SNAIL_PFTRIAD_MOCK_SLOT(AboutToDestroyView, IPfView);

  MOCK_METHOD1(createViewFor,
               std::shared_ptr<IPfView>(std::shared_ptr<IPfModel> model));
  MOCK_METHOD2(createViewFor,
               std::shared_ptr<IPfView>(
                   std::shared_ptr<IPfModel> model,
                   const IPfViewFactory::ViewFactoryIdType& vf_id));
  MOCK_METHOD1(removeTriadBy, void(IPfModel* model));
  MOCK_METHOD1(removeTriadBy, void(IPfView* view));
  MOCK_METHOD1(requestRemoveTriadByView, bool(IPfView* view));
  MOCK_CONST_METHOD1(findViewByModel,
                     std::vector<IPfView*>(IPfModel* model));
  MOCK_CONST_METHOD1(
      findViewsByModelId,
      std::vector<IPfView*>(const IPfModel::ModelIdType& model_id));
  MOCK_CONST_METHOD1(findModelByView, IPfModel*(IPfView* view));
};

}  // namespace tests
}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_MOCK_PF_TRIAD_MANAGER_H_
