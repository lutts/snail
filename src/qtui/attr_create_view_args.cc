// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/attr_create_view_args.h"

using namespace pfmvp;  // NOLINT

class AttrCreateViewArgsMemento : public pfmvp::PfCreateViewArgsMemento {
 public:
  explicit AttrCreateViewArgsMemento(bool edit_mode)
      : edit_mode_(edit_mode) { }
  virtual ~AttrCreateViewArgsMemento() = default;

  bool do_equals(const PfCreateViewArgsMemento& rhs) const override {
    const auto memento = dynamic_cast<const AttrCreateViewArgsMemento*>(&rhs);
    if (!memento)
      return false;

    return edit_mode_ == memento->edit_mode_;
  }

 private:
  bool edit_mode_;
};


AttrCreateViewArgs* AttrCreateViewArgs::getArgs(bool edit_mode) {
  static AttrCreateViewArgs edit_mode_args { true };
  static AttrCreateViewArgs display_mode_args { false };

  if (edit_mode) {
    return &edit_mode_args;
  } else {
    return &display_mode_args;
  }
}

std::unique_ptr<PfCreateViewArgsMemento>
AttrCreateViewArgs::createMemento() const {
  return utils::make_unique<AttrCreateViewArgsMemento>(edit_mode_);
}

bool AttrCreateViewArgs::do_memento_equals(
    const PfCreateViewArgsMemento& memento) const {
  AttrCreateViewArgsMemento temp_memento { edit_mode_ };
  return temp_memento.equals(memento);
}
