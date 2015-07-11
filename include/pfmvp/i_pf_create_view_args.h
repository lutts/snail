// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_I_PF_CREATE_VIEW_ARGS_H_
#define INCLUDE_PFMVP_I_PF_CREATE_VIEW_ARGS_H_

#include "utils/basic_utils.h"

namespace pfmvp {

class PfCreateViewArgsMemento {
 public:
  PfCreateViewArgsMemento() = default;
  virtual ~PfCreateViewArgsMemento() = default;

  void set_view_factory_id(
      const IPfViewFactory::ViewFactoryIdType& view_factory_id) {
    view_factory_id_ = view_factory_id;
  }

  IPfViewFactory::ViewFactoryIdType view_factory_id() const {
    return view_factory_id_;
  }

  bool equals(const PfCreateViewArgsMemento& rhs) const {
    if (!do_equals(rhs))
      return false;

    return view_factory_id_ == rhs.view_factory_id();
  }

 private:
  virtual bool do_equals(const PfCreateViewArgsMemento& rhs) const {
    (void)rhs;
    return true;
  }

 private:
  IPfViewFactory::ViewFactoryIdType view_factory_id_ {
    INVALID_PF_VIEW_FACTORY_ID };
};

class PfCreateViewArgs {
 public:
  PfCreateViewArgs() = default;
  virtual ~PfCreateViewArgs()  = default;

  void set_parent_presenter(PfPresenter* parent) {
    parent_presenter_ = parent;
  }

  PfPresenter* parent_presenter() const {
    return parent_presenter_;
  }

  void set_view_factory_id(
      const IPfViewFactory::ViewFactoryIdType& view_factory_id) {
    view_factory_id_ = view_factory_id;
  }

  IPfViewFactory::ViewFactoryIdType view_factory_id() const {
    return view_factory_id_;
  }

  void set_auto_remove_child(bool auto_remove_child) {
    auto_remove_child_ = auto_remove_child;
  }

  bool auto_remove_child() const {
    return auto_remove_child_;
  }

  std::unique_ptr<PfCreateViewArgsMemento> getMemento() const {
    auto memento = createMemento();

    initMemento(memento.get());

    return memento;
  }

  bool memento_equals(const PfCreateViewArgsMemento& memento) const {
    if (!do_memento_equals(memento))
      return false;

    PfCreateViewArgsMemento my_memento;
    initMemento(&my_memento);

    return my_memento.equals(memento);
  }

 private:
  virtual
  std::unique_ptr<PfCreateViewArgsMemento> createMemento() const {
    return utils::make_unique<PfCreateViewArgsMemento>();
  }

  virtual
  bool do_memento_equals(const PfCreateViewArgsMemento& memento) const {
    (void)memento;
    return true;
  }

 private:
  void initMemento(PfCreateViewArgsMemento* memento) const {
    // parent_presenter & auto_remove_child is not checked
    memento->set_view_factory_id(view_factory_id_);
  }

  IPfViewFactory::ViewFactoryIdType view_factory_id_ {
    INVALID_PF_VIEW_FACTORY_ID };
  PfPresenter* parent_presenter_ { nullptr };
  bool auto_remove_child_ { true };
};

}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_I_PF_CREATE_VIEW_ARGS_H_
