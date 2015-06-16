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

#define SNAIL_SIGSLOT_IMPL_(sigName)                                    \
  sigName##SignalType sigName;                                          \
                                                                        \
  void when##sigName(                                                   \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject) override {    \
    sigName##SignalType::slot_type subscriber(handler);                 \
        sigName.connect(subscriber.track_foreign(trackObject));         \
  }

#define SNAIL_SIGSLOT_IMPL(sigName)                                     \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_IMPL_(sigName)                                          \

#define SNAIL_SIGSLOT_COMBINER_IMPL(sigName, CombinerType)              \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerType>;    \
  SNAIL_SIGSLOT_IMPL_(sigName)                                          \

#define SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn_num)             \
  sigName##SignalType sigName;                                          \
                                                                        \
  void when##sigName(                                                   \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject) override {    \
    if (sigName.num_slots() >= max_conn_num) return;                    \
    sigName##SignalType::slot_type subscriber(handler);                 \
        sigName.connect(subscriber.track_foreign(trackObject));         \
  }

#define SNAIL_SIGSLOT_IMPL_MAX_CONN(sigName, max_conn_num)              \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn_num)                   \

#define SNAIL_SIGSLOT_COMBINER_IMPL_MAX_CONN(sigName, CombinerType, max_conn) \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerType>;    \
  SNAIL_SIGSLOT_IMPL_MAX_CONN_(sigName, max_conn)                       \

// pimpl macros
#define SNAIL_SIGSLOT_PIMPL_(sigName)                           \
  sigName##SignalType sigName;                                  \
                                                                \
  void when##sigName(                                           \
      sigName##SlotType handler,                                \
      std::shared_ptr<utils::ITrackable> trackObject) {     \
    sigName##SignalType::slot_type subscriber(handler);         \
        sigName.connect(subscriber.track_foreign(trackObject)); \
  }

#define SNAIL_SIGSLOT_PIMPL(PrimaryType, sigName)                       \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_PIMPL_(sigName)                                         \

#define SNAIL_SIGSLOT_COMBINER_PIMPL(PrimaryType, sigName, CombinerType) \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerType>;    \
  SNAIL_SIGSLOT_PIMPL_(sigName)                                         \


#define SNAIL_SIGSLOT_PIMPL_MAX_CONN_(sigName, max_conn)        \
  sigName##SignalType sigName;                                  \
                                                                \
  void when##sigName(                                           \
      sigName##SlotType handler,                                \
      std::shared_ptr<utils::ITrackable> trackObject) {     \
    sigName##SignalType::slot_type subscriber(handler);         \
      if (sigName.num_slots() >= max_conn) return;              \
      sigName.connect(subscriber.track_foreign(trackObject));   \
}

#define SNAIL_SIGSLOT_PIMPL_MAX_CONN(PrimaryType, sigName, max_conn)    \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT_PIMPL_MAX_CONN_(sigName, max_conn)                      \

#define SNAIL_SIGSLOT_COMBINER_PIMPL_MAX_CONN(PrimaryType, sigName,     \
                                              CombinerType, max_conn)   \
  using sigName##Signature = PrimaryType::sigName##Signature;           \
  using sigName##SlotType = PrimaryType::sigName##SlotType;             \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature, \
                                                      CombinerType>;    \
  SNAIL_SIGSLOT_PIMPL_MAX_CONN_(sigName, max_conn)                      \

#define SNAIL_SIGSLOT_PIMPL_RELAY(PrimaryType, sigName, pimpl)          \
  void PrimaryType::when##sigName(                                      \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject) {             \
    pimpl->when##sigName(handler, trackObject);                         \
  }

#endif  // INCLUDE_UTILS_SIGNAL_SLOT_IMPL_H_
