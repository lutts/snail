// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_GMOCK_EXT_GMOCK_EXT_H_
#define TEST_GMOCK_EXT_GMOCK_EXT_H_

#include <gmock/gmock.h>

#define MOCK_SPECIFIER_METHOD0(m, specifier, ...)       \
  GMOCK_METHOD0_(, specifier, , m, __VA_ARGS__)
#define MOCK_SPECIFIER_METHOD1(m, specifier, ...)       \
  GMOCK_METHOD1_(, specifier, , m, __VA_ARGS__)
#define MOCK_SPECIFIER_METHOD2(m, specifier, ...)       \
  GMOCK_METHOD2_(, specifier, , m, __VA_ARGS__)
#define MOCK_SPECIFIER_METHOD3(m, specifier, ...)       \
  GMOCK_METHOD3_(, specifier, , m, __VA_ARGS__)
#define MOCK_SPECIFIER_METHOD4(m, specifier, ...)       \
  GMOCK_METHOD4_(, specifier, , m, __VA_ARGS__)
#define MOCK_SPECIFIER_METHOD5(m, specifier, ...)       \
  GMOCK_METHOD5_(, specifier, , m, __VA_ARGS__)
#define MOCK_SPECIFIER_METHOD6(m, specifier, ...)       \
  GMOCK_METHOD6_(, specifier, , m, __VA_ARGS__)
#define MOCK_SPECIFIER_METHOD7(m, specifier, ...)       \
  GMOCK_METHOD7_(, specifier, , m, __VA_ARGS__)
#define MOCK_SPECIFIER_METHOD8(m, specifier, ...)       \
  GMOCK_METHOD8_(, specifier, , m, __VA_ARGS__)
#define MOCK_SPECIFIER_METHOD9(m, specifier, ...)       \
  GMOCK_METHOD9_(, specifier, , m, __VA_ARGS__)
#define MOCK_SPECIFIER_METHOD10(m, specifier, ...)      \
  GMOCK_METHOD10_(, specifier, , m, __VA_ARGS__)

#endif  // TEST_GMOCK_EXT_GMOCK_EXT_H_
