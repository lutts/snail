// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_I_PF_VIEW_H_
#define INCLUDE_PFMVP_I_PF_VIEW_H_

namespace pfmvp {

class IPfView {
 public:
  virtual ~IPfView() = default;

  // @return: for dialog views, true for user accepted, false for rejected
  //          for other views, the meaning of the result is undefined
  virtual bool showView(bool modal = false) {
    (void)modal;
    return false;
  }
  virtual void onDestroy() {}
};

}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_I_PF_VIEW_H_
