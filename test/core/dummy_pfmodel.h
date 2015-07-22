// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_CORE_DUMMY_PFMODEL_H_
#define TEST_CORE_DUMMY_PFMODEL_H_

#include "pfmvp/i_pf_model.h"

namespace snailcore {
namespace tests {

class DummyModel : public pfmvp::IPfModel {
 public:
  virtual ~DummyModel() = default;

  DEF_MODEL_ID(DummyModel);
};

}  // namespace tests
}  // namespace snailcore

#endif  // TEST_CORE_DUMMY_PFMODEL_H_
