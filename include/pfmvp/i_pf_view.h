// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_PF_VIEW_H_
#define INCLUDE_SNAIL_I_PF_VIEW_H_

namespace pfmvp {

class IPfView {
 public:
  virtual ~IPfView() = default;

  virtual void showView(bool modal = false) { (void)modal; }
};

}  // namespace pfmvp

#endif  // INCLUDE_SNAIL_I_PF_VIEW_H_
