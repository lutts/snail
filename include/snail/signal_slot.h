// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_SIGNAL_SLOT_H_
#define INCLUDE_SNAIL_SIGNAL_SLOT_H_

#include <functional>  // std::function
#include <memory>      // std::shared_ptr
#include "snail/i_trackable.h"  // ITrackable

#define SNAIL_SIGSLOT2(sigName, ...)                                    \
  using sigName##Signature = __VA_ARGS__;                               \
  using sigName##SlotType = std::function<sigName##Signature>;          \
  virtual void when##sigName(                                           \
      sigName##SlotType handler,                                        \
      std::shared_ptr<snailcore::ITrackable> trackObject) = 0

#define SNAIL_MOCK_SLOT(sigName)                                        \
  MOCK_METHOD2(when##sigName,                                           \
               void(sigName##SlotType, std::shared_ptr<snailcore::ITrackable>))

// as its name suggests, this macro is used For Test Only, this macro has
// a default implementation for the whenXXX connection point, so you do not
// have to modify a derived class of an interface just because you added a new
// signal
#define SNAIL_SIGSLOT_FTO(sigName, ...)                         \
  using sigName##Signature = __VA_ARGS__;                       \
  using sigName##SlotType = std::function<sigName##Signature>;  \
  virtual void when##sigName(                                   \
      sigName##SlotType handler,                                \
      std::shared_ptr<snailcore::ITrackable> trackObject) {     \
    (void)handler;                                              \
    (void)trackObject;                                          \
  }

namespace utils {

// NOTE: don't use make_shared, boost.signal2 track feature will cause memory
// leak with make_shared
// If you are very sure that the signal provider will also be deleted soon, you
// can use std::make_shared directly
template<typename T, typename... Args>
std::shared_ptr<T> make_trackable(Args&&... args) {
  return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
}

}  // namespace utils

#endif  // INCLUDE_SNAIL_SIGNAL_SLOT_H_
