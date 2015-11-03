// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_SIGNAL_SLOT_H_
#define INCLUDE_UTILS_SIGNAL_SLOT_H_

#include <boost/version.hpp>

#include <functional>  // std::function
#include <memory>      // std::shared_ptr
#include "utils/i_trackable.h"  // ITrackable

// NOTE: should ignore trackObject param only when you can ensure the subscriber
// has a longer lifetime than the subject.

#define SNAIL_SIGSLOT_PURE_VIRTUAL(sigName, ...)                        \
  using sigName##Signature = __VA_ARGS__;                               \
  using sigName##SlotType = std::function<sigName##Signature>;          \
  virtual void when##sigName(                                           \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject = nullptr) = 0;    \
  virtual void cleanup##sigName##Slots() = 0;

#define SNAIL_MOCK_SLOT(sigName)                                        \
  MOCK_METHOD2(when##sigName,                                           \
               void(sigName##SlotType, std::shared_ptr<utils::ITrackable>)); \
  MOCK_METHOD0(cleanup##sigName##Slots, void());

#define SNAIL_SIGSLOT_PROXY(PrimaryType, sigName)       \
  void when##sigName(                                   \
      PrimaryType::sigName##SlotType handler,           \
      std::shared_ptr<utils::ITrackable> trackObject) { \
    self_->when##sigName(handler, trackObject);         \
  }                                                     \
  void cleanup##sigName##Slots() {                      \
    self_->cleanup##sigName##Slots();                   \
  }

#define SNAIL_SIGSLOT_NONVIRTUAL(sigName, ...)                          \
  using sigName##Signature = __VA_ARGS__;                               \
  using sigName##SlotType = std::function<sigName##Signature>;          \
  void when##sigName(                                                   \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject = nullptr);        \
  void cleanup##sigName##Slots();

#define SNAIL_OVERRIDE_SLOT(sigName)                                  \
  void when##sigName(                                                 \
      sigName##SlotType handler,                                      \
      std::shared_ptr<utils::ITrackable> trackObject) override;       \
  void cleanup##sigName##Slots() override;

// as its name suggests, this macro is used For Test Only, this macro has
// a default implementation for the whenXXX connection point, so you do not
// have to modify a derived class of an interface just because you added a new
// signal
#define SNAIL_SIGSLOT_FTO(sigName, ...)                                 \
  using sigName##Signature = __VA_ARGS__;                               \
  using sigName##SlotType = std::function<sigName##Signature>;          \
  virtual void when##sigName(                                           \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject = nullptr) {       \
    (void)handler;                                                      \
    (void)trackObject;                                                  \
  }                                                                     \
  virtual void cleanup##sigName##Slots() { }                            \

namespace utils {

// NOTE: don't use make_shared, boost.signal2 track feature will cause memory
// leak with make_shared
// If you are very sure that the signal provider will also be deleted soon, you
// can use std::make_shared directly
// NOTE: from boost 1.59.0, memory leak problem can be avoid by cleanup
// expired connection by calling num_slots or connection.disconnect
#if BOOST_VERSION >= 105900
template<typename T, typename... Args>
std::shared_ptr<T> make_trackable(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}
#else  // < 1.59.0
template<typename T, typename... Args>
std::shared_ptr<T> make_trackable(Args&&... args) {
  return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif  // BOOST_VERSION < 1.59.0

}  // namespace utils

#endif  // INCLUDE_UTILS_SIGNAL_SLOT_H_
