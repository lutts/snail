// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SIMPLE_MOCK_LISTENER_H_
#define SIMPLE_MOCK_LISTENER_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "utils/basic_utils.h"
#include "test/function_signature_extracter.h"

/**
 * Mock listeners should not be tracked in theory, because if mock listeners
 * are destroyed, it means that we do not expect any signals anymore. this
 * is not the same as StrictMock, StrictMock means when the mock is still
 * existing, we do not want unexpected signals, but do not track mock listener
 * means we do not want unexpected signals after the mock is destroyed
 *
 * Note that StrictMock is still prefered because it can tell which signal
 * is unexpected emitted.
 *
 * we also provide detach calls to allow users detach the listener if they
 * do not care whether signals will emited or not.
 */

template <typename SubjectType>
class SimpleMockListener {
 public:
  class IListener {
   public:
    virtual ~IListener() = default;

    virtual void attach() = 0;
    virtual void detatch() = 0;
  };

  SimpleMockListener(SubjectType *subject) : subject_(subject) {}

  void attach() {
    if (attached_) return;

    for (auto &listener : listeners_) {
      listener->attach();
    }
    attached_ = true;
  }

  void detatch() {
    if (!attached_) return;

    for (auto &listener : listeners_) {
      listener->detatch();
    }
    attached_ = false;
  }

  SubjectType *getSubject() { return subject_; }

 protected:
  SubjectType *subject_;
  std::vector<IListener *> listeners_;
  bool attached_{false};

 private:
  SNAIL_DISABLE_COPY(SimpleMockListener)
};

#define SNAIL_MOCK_LISTENER_VARIABLE(Name) Name##Listener_
#define SNAIL_MOCK_LISTENER_REGISTER(Name, THIS)             \
  listeners_.push_back(&SNAIL_MOCK_LISTENER_VARIABLE(Name)); \
  SNAIL_MOCK_LISTENER_VARIABLE(Name).setMockListener(THIS);

#define SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Name) \
  class Name##Listener : public IListener {       \
   public:                                        \
    void setMockListener(ML *mock_listener) {     \
      mock_listener_ = mock_listener;             \
    }                                             \
    void attach() override {                      \
    conn_ = mock_listener_->getSubject()->
#define SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Name)  \
  }                                               \
                                                  \
  void detatch() override { conn_.disconnect(); } \
                                                  \
 private:                                         \
  ML *mock_listener_;                             \
  SignalConnection conn_;                         \
  }                                               \
  ;                                               \
  Name##Listener SNAIL_MOCK_LISTENER_VARIABLE(Name);

#define SNAIL_MOCK_LISTENER0(ML, Method, ...)                           \
  MOCK_METHOD0(Method, __VA_ARGS__);                                    \
  SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Method)                           \
  when##Method([this]() { return mock_listener_->Method(); }, nullptr); \
  SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Method)

#define SNAIL_MOCK_LISTENER1(ML, Method, ...)            \
  MOCK_METHOD1(Method, __VA_ARGS__);                     \
  SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Method)            \
  when##Method([this](SNAIL_ARG_(, 1, __VA_ARGS__) a1) { \
    return mock_listener_->Method(a1);                   \
  }, nullptr);                                           \
  SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Method)

#define SNAIL_MOCK_LISTENER2(ML, Method, ...)                             \
  MOCK_METHOD2(Method, __VA_ARGS__);                                      \
  SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Method)                             \
  when##Method([this](SNAIL_ARG_(, 1, __VA_ARGS__) a1,                    \
                      SNAIL_ARG_(, 2, __VA_ARGS__)                        \
                          a2) { return mock_listener_->Method(a1, a2); }, \
               nullptr);                                                  \
  SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Method)

#define SNAIL_MOCK_LISTENER3(ML, Method, ...)                                  \
  MOCK_METHOD3(Method, __VA_ARGS__);                                           \
  SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Method)                                  \
  when##Method(                                                                \
      [this](SNAIL_ARG_(, 1, __VA_ARGS__) a1, SNAIL_ARG_(, 2, __VA_ARGS__) a2, \
             SNAIL_ARG_(, 3, __VA_ARGS__)                                      \
                 a3) { return mock_listener_->Method(a1, a2, a3); },           \
      nullptr);                                                                \
  SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Method)

#define SNAIL_MOCK_LISTENER4(ML, Method, ...)                                  \
  MOCK_METHOD4(Method, __VA_ARGS__);                                           \
  SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Method)                                  \
  when##Method(                                                                \
      [this](SNAIL_ARG_(, 1, __VA_ARGS__) a1, SNAIL_ARG_(, 2, __VA_ARGS__) a2, \
             SNAIL_ARG_(, 3, __VA_ARGS__) a3,                                  \
             SNAIL_ARG_(, 4, __VA_ARGS__)                                      \
                 a4) { return mock_listener_->Method(a1, a2, a3, a4); },       \
      nullptr);                                                                \
  SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Method)

#define SNAIL_MOCK_LISTENER5(ML, Method, ...)                                  \
  MOCK_METHOD5(Method, __VA_ARGS__);                                           \
  SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Method)                                  \
  when##Method(                                                                \
      [this](SNAIL_ARG_(, 1, __VA_ARGS__) a1, SNAIL_ARG_(, 2, __VA_ARGS__) a2, \
             SNAIL_ARG_(, 3, __VA_ARGS__) a3, SNAIL_ARG_(, 4, __VA_ARGS__) a4, \
             SNAIL_ARG_(, 5, __VA_ARGS__)                                      \
                 a5) { return mock_listener_->Method(a1, a2, a3, a4, a5); },   \
      nullptr);                                                                \
  SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Method)

#define SNAIL_MOCK_LISTENER6(ML, Method, ...)                                  \
  MOCK_METHOD6(Method, __VA_ARGS__);                                           \
  SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Method)                                  \
  when##Method(                                                                \
      [this](SNAIL_ARG_(, 1, __VA_ARGS__) a1, SNAIL_ARG_(, 2, __VA_ARGS__) a2, \
             SNAIL_ARG_(, 3, __VA_ARGS__) a3, SNAIL_ARG_(, 4, __VA_ARGS__) a4, \
             SNAIL_ARG_(, 5, __VA_ARGS__) a5,                                  \
             SNAIL_ARG_(, 6, __VA_ARGS__) a6) {                                \
        return mock_listener_->Method(a1, a2, a3, a4, a5, a6);                 \
      },                                                                       \
      nullptr);                                                                \
  SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Method)

#define SNAIL_MOCK_LISTENER7(ML, Method, ...)                                  \
  MOCK_METHOD7(Method, __VA_ARGS__);                                           \
  SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Method)                                  \
  when##Method(                                                                \
      [this](SNAIL_ARG_(, 1, __VA_ARGS__) a1, SNAIL_ARG_(, 2, __VA_ARGS__) a2, \
             SNAIL_ARG_(, 3, __VA_ARGS__) a3, SNAIL_ARG_(, 4, __VA_ARGS__) a4, \
             SNAIL_ARG_(, 5, __VA_ARGS__) a5, SNAIL_ARG_(, 6, __VA_ARGS__) a6, \
             SNAIL_ARG_(, 7, __VA_ARGS__) a7) {                                \
        return mock_listener_->Method(a1, a2, a3, a4, a5, a6, a7);             \
      },                                                                       \
      nullptr);                                                                \
  SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Method)

#define SNAIL_MOCK_LISTENER8(ML, Method, ...)                                  \
  MOCK_METHOD8(Method, __VA_ARGS__);                                           \
  SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Method)                                  \
  when##Method(                                                                \
      [this](SNAIL_ARG_(, 1, __VA_ARGS__) a1, SNAIL_ARG_(, 2, __VA_ARGS__) a2, \
             SNAIL_ARG_(, 3, __VA_ARGS__) a3, SNAIL_ARG_(, 4, __VA_ARGS__) a4, \
             SNAIL_ARG_(, 5, __VA_ARGS__) a5, SNAIL_ARG_(, 6, __VA_ARGS__) a6, \
             SNAIL_ARG_(, 7, __VA_ARGS__) a7,                                  \
             SNAIL_ARG_(, 8, __VA_ARGS__) a8) {                                \
        return mock_listener_->Method(a1, a2, a3, a4, a5, a6, a7, a8);         \
      },                                                                       \
      nullptr);                                                                \
  SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Method)

#define SNAIL_MOCK_LISTENER9(ML, Method, ...)                                  \
  MOCK_METHOD9(Method, __VA_ARGS__);                                           \
  SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Method)                                  \
  when##Method(                                                                \
      [this](SNAIL_ARG_(, 1, __VA_ARGS__) a1, SNAIL_ARG_(, 2, __VA_ARGS__) a2, \
             SNAIL_ARG_(, 3, __VA_ARGS__) a3, SNAIL_ARG_(, 4, __VA_ARGS__) a4, \
             SNAIL_ARG_(, 5, __VA_ARGS__) a5, SNAIL_ARG_(, 6, __VA_ARGS__) a6, \
             SNAIL_ARG_(, 7, __VA_ARGS__) a7, SNAIL_ARG_(, 8, __VA_ARGS__) a8, \
             SNAIL_ARG_(, 9, __VA_ARGS__) gmock_a9) {                          \
        return mock_listener_->Method(a1, a2, a3, a4, a5, a6, a7, a8, a9);     \
      },                                                                       \
      nullptr);                                                                \
  SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Method)

#define SNAIL_MOCK_LISTENER10(ML, Method, ...)                                 \
  MOCK_METHOD10(Method, __VA_ARGS__);                                          \
  SNAIL_MOCK_LISTENER_BODY_BEFORE(ML, Method)                                  \
  when##Method(                                                                \
      [this](SNAIL_ARG_(, 1, __VA_ARGS__) a1, SNAIL_ARG_(, 2, __VA_ARGS__) a2, \
             SNAIL_ARG_(, 3, __VA_ARGS__) a3, SNAIL_ARG_(, 4, __VA_ARGS__) a4, \
             SNAIL_ARG_(, 5, __VA_ARGS__) a5, SNAIL_ARG_(, 6, __VA_ARGS__) a6, \
             SNAIL_ARG_(, 7, __VA_ARGS__) a7, SNAIL_ARG_(, 8, __VA_ARGS__) a8, \
             SNAIL_ARG_(, 9, __VA_ARGS__) a9,                                  \
             SNAIL_ARG_(, 10, __VA_ARGS__) a10) {                              \
        return mock_listener_->Method(a1, a2, a3, a4, a5, a6, a7, a8, a9,      \
                                      a10);                                    \
      },                                                                       \
      nullptr);                                                                \
  SNAIL_MOCK_LISTENER_BODY_AFTER(ML, Method)

#endif  // SIMPLE_MOCK_LISTENER_H_
