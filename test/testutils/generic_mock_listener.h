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
class GenericMockListener : public utils::ITrackable {
 public:
  template <typename MockType>
  static std::shared_ptr<MockType>
  attachTo_(SubjectType* subject) {
    auto listener = std::make_shared<MockType>();

    listener->bindListenerMethods(listener, subject);

    return listener;
  }

  static std::shared_ptr<ListenerType>
  attachTo(SubjectType* subject) {
    return attachTo_<ListenerType>(subject);
  }

  static std::shared_ptr<StrictMock<ListenerType>>
  attachStrictTo(SubjectType* subject) {
    return attachTo_<StrictMock<ListenerType> >(subject);
  }

  template <typename MockType, typename ... Args>
  static std::shared_ptr<MockType>
  attachTo_(SubjectType* subject, Args&& ... args) {
    auto listener = std::make_shared<MockType>(std::forward<Args> (args) ...);

    listener->bindListenerMethods(listener, subject);

    return listener;
  }

  template <typename ... Args>
  static std::shared_ptr<ListenerType>
  attachTo(SubjectType* subject, Args&& ... args) {
    return attachTo_<ListenerType, Args...>(subject,
                                            std::forward<Args> (args) ...);
  }

  template <typename ... Args>
  static std::shared_ptr<StrictMock<ListenerType>>
  attachStrictTo(SubjectType* subject, Args&& ... args) {
    return attachTo_<StrictMock<ListenerType>, Args... >(
        subject,
        std::forward<Args> (args) ...);
  }

  virtual void bindListenerMethods(
      std::shared_ptr<utils::ITrackable> trackObject,
      SubjectType* subject) = 0;
};

#endif  // TEST_TESTUTILS_GENERIC_MOCK_LISTENER_H_
