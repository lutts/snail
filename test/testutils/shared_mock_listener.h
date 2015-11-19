// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_GENERIC_MOCK_LISTENER_H_
#define TEST_TESTUTILS_GENERIC_MOCK_LISTENER_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>  // std::shared_ptr

#include "utils/i_trackable.h"  // ITrackable

using ::testing::StrictMock;

template <typename ListenerType, typename SubjectType>
class SharedMockListener : public utils::ITrackable {
 public:
  template <typename MockType>
  static std::shared_ptr<MockType> attachTo_(SubjectType* subject) {
    auto listener = std::make_shared<MockType>();

    listener->bindListenerMethods(listener, subject);

    return listener;
  }

  static std::shared_ptr<ListenerType> attachTo(SubjectType* subject) {
    return attachTo_<ListenerType>(subject);
  }

  static std::shared_ptr<StrictMock<ListenerType>> attachStrictTo(
      SubjectType* subject) {
    return attachTo_<StrictMock<ListenerType>>(subject);
  }

  template <typename MockType, typename... Args>
  static std::shared_ptr<MockType> attachTo_(SubjectType* subject,
                                             Args&&... args) {
    auto listener = std::make_shared<MockType>(std::forward<Args>(args)...);

    listener->bindListenerMethods(listener, subject);

    return listener;
  }

  template <typename... Args>
  static std::shared_ptr<ListenerType> attachTo(SubjectType* subject,
                                                Args&&... args) {
    return attachTo_<ListenerType, Args...>(subject,
                                            std::forward<Args>(args)...);
  }

  template <typename... Args>
  static std::shared_ptr<StrictMock<ListenerType>> attachStrictTo(
      SubjectType* subject, Args&&... args) {
    return attachTo_<StrictMock<ListenerType>, Args...>(
        subject, std::forward<Args>(args)...);
  }

  virtual void bindListenerMethods(
      std::shared_ptr<utils::ITrackable> trackObject, SubjectType* subject) = 0;
};

#define BIND_SIGNAL0(sigName, RetType) \
  subject->when##sigName([this]() -> RetType { return sigName(); }, trackObject)

#define BIND_SIGNAL1(sigName, RetType, Arg1Type, Arg1Var)      \
  subject->when##sigName([this](Arg1Type Arg1Var) -> RetType { \
    return sigName(Arg1Var);                                   \
  }, trackObject)

#define BIND_SIGNAL2(sigName, RetType, Arg1Type, Arg1Var, Arg2Type, Arg2Var) \
  subject->when##sigName(                                                    \
      [this](Arg1Type Arg1Var, Arg2Type Arg2Var) -> RetType {                \
        return sigName(Arg1Var, Arg2Var);                                    \
      },                                                                     \
      trackObject)

#define BIND_SIGNAL3(sigName, RetType, Arg1Type, Arg1Var, Arg2Type, Arg2Var, \
                     Arg3Type, Arg3Var)                                      \
  subject->when##sigName(                                                    \
      [this](Arg1Type Arg1Var, Arg2Type Arg2Var, Arg3Type Arg3Var)           \
          -> RetType { return sigName(Arg1Var, Arg2Var, Arg3Var); },         \
      trackObject)

#define BEGIN_MOCK_LISTENER_DEF(ClassName, SubjectType) \
  struct ClassName : public SharedMockListener<ClassName, SubjectType> {
// }

// clang-format off
#define END_MOCK_LISTENER_DEF() };
// clang-format on

#define BEGIN_BIND_SIGNAL(SubjectType)                                     \
  void bindListenerMethods(std::shared_ptr<utils::ITrackable> trackObject, \
                           SubjectType* subject) {
// }

#define END_BIND_SIGNAL() }

#endif  // TEST_TESTUTILS_GENERIC_MOCK_LISTENER_H_
