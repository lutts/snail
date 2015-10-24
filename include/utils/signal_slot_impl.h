// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_SIGNAL_SLOT_IMPL_H_
#define INCLUDE_UTILS_SIGNAL_SLOT_IMPL_H_

#include <boost/signals2.hpp>
#include <memory>      // std::shared_ptr

#define SNAIL_SIGSLOT_IMPL_(sigName, ovrd)                              \
  sigName##SignalType sigName;                                          \
                                                                        \
  void when##sigName(                                                   \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject) ovrd {            \
    sigName##SignalType::slot_type subscriber(handler);                 \
        if (trackObject)                                                \
          sigName.connect(subscriber.track_foreign(trackObject));       \
        else                                                            \
          sigName.connect(subscriber);                                  \
  }                                                                     \
  void cleanup##sigName##Slots() ovrd {                                 \
    sigName.num_slots();                                                \
  }

#define SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn_num, ovrd)       \
  sigName##SignalType sigName;                                          \
                                                                        \
  void when##sigName(                                                   \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject) ovrd {            \
    if (sigName.num_slots() >= max_conn_num) return;                    \
    sigName##SignalType::slot_type subscriber(handler);                 \
        if (trackObject)                                                \
          sigName.connect(subscriber.track_foreign(trackObject));       \
        else                                                            \
          sigName.connect(subscriber);                                  \
  }                                                                     \
  void cleanup##sigName##Slots() ovrd {                                 \
    sigName.num_slots();                                                \
  }

// non override impls (default)
#define SNAIL_SIGSLOT_IMPL(sigName, ...)                                \
  using sigName##Signature = __VA_ARGS__;                               \
  using sigName##SlotType = std::function<sigName##Signature>;          \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_IMPL_(sigName, )                                        \

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
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn_num, )                 \

#define SNAIL_SIGSLOT_COMBINER_MAX_CONN_IMPL(sigName, CombinerT, maxconn, ...) \
  using sigName##Signature = __VA_ARGS__;                               \
  using sigName##SlotType = std::function<sigName##Signature>;          \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerT>;       \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, maxconn, )                      \

// override impls
#define SNAIL_SIGSLOT_OVERRIDE(sigName)                                 \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_IMPL_(sigName, override)                                \

#define SNAIL_SIGSLOT_COMBINER_OVERRIDE(sigName, CombinerType)          \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerType>;    \
  SNAIL_SIGSLOT_IMPL_(sigName, override)

#define SNAIL_SIGSLOT_MAX_CONN_OVERRIDE(sigName, max_conn_num)          \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn_num, override)         \

#define SNAIL_SIGSLOT_COMBINER_MAX_CONN_OVERRIDE(sigName, CombinerT, max_conn) \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerT>;       \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn, override)

// pimpl macros
#define SNAIL_SIGSLOT_PIMPL(PrimaryType, sigName)                       \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_IMPL_(sigName, )                                        \

#define SNAIL_SIGSLOT_COMBINER_PIMPL(PrimaryType, sigName, CombinerType) \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerType>;    \
  SNAIL_SIGSLOT_IMPL_(sigName, )

#define SNAIL_SIGSLOT_MAX_CONN_PIMPL(PrimaryType, sigName, max_conn)    \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn, )                     \

#define SNAIL_SIGSLOT_COMBINER_MAX_CONN_PIMPL(PrimaryType, sigName,     \
                                              CombinerType, max_conn)   \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerType>;    \
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

#endif  // INCLUDE_UTILS_SIGNAL_SLOT_IMPL_H_
