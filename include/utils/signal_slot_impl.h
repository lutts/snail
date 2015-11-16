// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_SIGNAL_SLOT_IMPL_H_
#define INCLUDE_UTILS_SIGNAL_SLOT_IMPL_H_

#include <boost/signals2.hpp>

#include "utils/signal_slot.h"

// NOTE: signal do not allow copy, pls see boost.signal2 document

template <typename SignalType>
class SignalConnectionHelper {
 public:
  static void connectSignal(SignalType& sig,  // NOLINT
                            typename SignalType::slot_type subscriber,
                            std::shared_ptr<utils::ITrackable> trackObject) {
    if (trackObject)
      sig.connect(subscriber.track_foreign(trackObject));
    else
      sig.connect(subscriber);
  }

  static void connectSignal(SignalType& sig,  // NOLINT
                            typename SignalType::slot_type subscriber,
                            std::shared_ptr<utils::ITrackable> trackObject,
                            int max_connections) {
    // TODO(lutts): do we need lock?
    if (sig.num_slots() >= max_connections) return;
    connectSignal(sig, subscriber, trackObject);
  }

  static void cleanupSignal(SignalType& sig) {  // NOLINT
    // call num_slots() will cleanup expired connections, this is a WorkAround!
    sig.num_slots();
  }

 private:
  SignalConnectionHelper() = delete;
  ~SignalConnectionHelper() = delete;
};

#define SNAIL_SIGSLOT_IMPL_(sigName, ovrd)                              \
  void when##sigName(                                                   \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject) ovrd {            \
    SignalConnectionHelper<sigName##SignalType>::connectSignal(sigName, \
                                                               handler, \
                                                               trackObject); \
  }                                                                     \
  void cleanup##sigName##Slots() ovrd {                                 \
    SignalConnectionHelper<sigName##SignalType>::cleanupSignal(sigName); \
  }                                                                     \
 private:                                                               \
  sigName##SignalType sigName { };

#define SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn_num, ovrd)       \
  void when##sigName(                                                   \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject) ovrd {            \
    SignalConnectionHelper<sigName##SignalType>::connectSignal(sigName, \
                                                               handler, \
                                                               trackObject, \
                                                               max_conn_num); \
  }                                                                     \
  void cleanup##sigName##Slots() ovrd {                                 \
    SignalConnectionHelper<sigName##SignalType>::cleanupSignal(sigName); \
  }                                                                     \
 private:                                                               \
  sigName##SignalType sigName { };

// non override impls (default)
#define SNAIL_SIGSLOT_IMPL(sigName, ...)                                \
  using sigName##Signature = __VA_ARGS__;                               \
  using sigName##SlotType = std::function<sigName##Signature>;          \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_IMPL_(sigName, )

#define SNAIL_SIGSLOT_COMBINER_IMPL(sigName, CombinerType, ...)         \
  using sigName##Signature = __VA_ARGS__;                               \
  using sigName##SlotType = std::function<sigName##Signature>;          \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerType>;    \
  SNAIL_SIGSLOT_IMPL_(sigName, )

#define SNAIL_SIGSLOT_MAX_CONN_IMPL(sigName, max_conn_num, ...)         \
  using sigName##Signature = __VA_ARGS__;                               \
  using sigName##SlotType = std::function<sigName##Signature>;          \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn_num, )

#define SNAIL_SIGSLOT_COMBINER_MAX_CONN_IMPL(sigName, CombinerT, maxconn, ...) \
  using sigName##Signature = __VA_ARGS__;                               \
  using sigName##SlotType = std::function<sigName##Signature>;          \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerT>;       \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, maxconn, )

// override impls
#define SNAIL_SIGSLOT_OVERRIDE_IMPL(sigName)                                 \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_IMPL_(sigName, override)                                \

#define SNAIL_SIGSLOT_COMBINER_OVERRIDE_IMPL(sigName, CombinerType)          \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerType>;    \
  SNAIL_SIGSLOT_IMPL_(sigName, override)

#define SNAIL_SIGSLOT_MAX_CONN_OVERRIDE_IMPL(sigName, max_conn_num)          \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn_num, override)         \

#define SNAIL_SIGSLOT_COMBINER_MAX_CONN_OVERRIDE_IMPL(sigName, CombinerT, \
                                                      max_conn)         \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerT>;       \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn, override)

// pimpl macros
#define SNAIL_SIGSLOT_PIMPL(PrimaryType, sigName)                       \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  template <typename ... Args>                                          \
  sigName##SlotType::result_type emit##sigName(Args&& ... args) {       \
    return sigName(std::forward<Args>(args)...);                        \
  }                                                                     \
  SNAIL_SIGSLOT_IMPL_(sigName, )

#define SNAIL_SIGSLOT_COMBINER_PIMPL(PrimaryType, sigName, CombinerType) \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerType>;    \
  template <typename ... Args>                                          \
  sigName##SlotType::result_type emit##sigName(Args&& ... args) {       \
    return sigName(std::forward<Args>(args)...);                        \
  }                                                                     \
  SNAIL_SIGSLOT_IMPL_(sigName, )

#define SNAIL_SIGSLOT_MAX_CONN_PIMPL(PrimaryType, sigName, max_conn)    \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  template <typename ... Args>                                          \
  sigName##SlotType::result_type emit##sigName(Args&& ... args) {       \
    return sigName(std::forward<Args>(args)...);                        \
  }                                                                     \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn, )


#define SNAIL_SIGSLOT_COMBINER_MAX_CONN_PIMPL(PrimaryType, sigName,     \
                                              CombinerType, max_conn)   \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerType>;    \
  template <typename ... Args>                                          \
  sigName##SlotType::result_type emit##sigName(Args&& ... args) {       \
    return sigName(std::forward<Args>(args)...);                        \
  }                                                                     \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn, )


#define SNAIL_SIGSLOT_DELEGATE(PrimaryType, sigName, sigProxy)  \
  void PrimaryType::when##sigName(                              \
      sigName##SlotType handler,                                \
      std::shared_ptr<utils::ITrackable> trackObject) {         \
    sigProxy->when##sigName(handler, trackObject);              \
  }                                                             \
  void PrimaryType::cleanup##sigName##Slots() {                 \
    sigProxy->cleanup##sigName##Slots();                        \
  }

#define SNAIL_SIGSLOT_DELEGATE2(PrimaryType, sigName)   \
  SNAIL_SIGSLOT_DELEGATE(PrimaryType, sigName, signal_helper_)

#endif  // INCLUDE_UTILS_SIGNAL_SLOT_IMPL_H_
