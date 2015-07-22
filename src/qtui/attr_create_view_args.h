// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_ATTR_CREATE_VIEW_ARGS_H_
#define SRC_QTUI_ATTR_CREATE_VIEW_ARGS_H_

#include <memory>

#include "pfmvp/i_pf_create_view_args.h"

class AttrCreateViewArgs : public pfmvp::PfCreateViewArgs {
 public:
  explicit AttrCreateViewArgs(bool edit_mode) : edit_mode_(edit_mode) { }
  virtual ~AttrCreateViewArgs() = default;

  static AttrCreateViewArgs* getArgs(bool edit_mode);

  bool isEditMode() {
    return edit_mode_;
  }

  std::unique_ptr<pfmvp::PfCreateViewArgsMemento>
  createMemento() const override;

  bool do_memento_equals(
      const pfmvp::PfCreateViewArgsMemento& memento) const override;

 private:
  bool edit_mode_;
};


#endif  // SRC_QTUI_ATTR_CREATE_VIEW_ARGS_H_
