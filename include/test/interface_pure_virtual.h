// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_TEST_INTERFACE_PURE_VIRTUAL_H_
#define INCLUDE_TEST_INTERFACE_PURE_VIRTUAL_H_

#include "test/function_signature_extracter.h"

#define SNAIL_INTERFACE0_(tn, constness, ct, Method, ...) \
  virtual SNAIL_RESULT_(tn, __VA_ARGS__) ct Method() constness = 0

#define SNAIL_INTERFACE1_(tn, constness, ct, Method, ...) \
  virtual SNAIL_RESULT_(tn, __VA_ARGS__)                  \
      ct Method(SNAIL_ARG_(tn, 1, __VA_ARGS__) a1) constness = 0

#define SNAIL_INTERFACE2_(tn, constness, ct, Method, ...)                   \
  virtual SNAIL_RESULT_(tn, __VA_ARGS__) ct Method(                         \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2) \
      constness = 0

#define SNAIL_INTERFACE3_(tn, constness, ct, Method, ...)                   \
  virtual SNAIL_RESULT_(tn, __VA_ARGS__) ct Method(                         \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3) constness = 0

#define SNAIL_INTERFACE4_(tn, constness, ct, Method, ...)                   \
  virtual SNAIL_RESULT_(tn, __VA_ARGS__) ct Method(                         \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4) \
      constness = 0

#define SNAIL_INTERFACE5_(tn, constness, ct, Method, ...)                   \
  virtual SNAIL_RESULT_(tn, __VA_ARGS__) ct Method(                         \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4, \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5) constness = 0

#define SNAIL_INTERFACE6_(tn, constness, ct, Method, ...)                   \
  virtual SNAIL_RESULT_(tn, __VA_ARGS__) ct Method(                         \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4, \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5, SNAIL_ARG_(tn, 6, __VA_ARGS__) a6) \
      constness = 0

#define SNAIL_INTERFACE7_(tn, constness, ct, Method, ...)                   \
  virtual SNAIL_RESULT_(tn, __VA_ARGS__) ct Method(                         \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4, \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5, SNAIL_ARG_(tn, 6, __VA_ARGS__) a6, \
      SNAIL_ARG_(tn, 7, __VA_ARGS__) a7) constness = 0

#define SNAIL_INTERFACE8_(tn, constness, ct, Method, ...)                   \
  virtual SNAIL_RESULT_(tn, __VA_ARGS__) ct Method(                         \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4, \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5, SNAIL_ARG_(tn, 6, __VA_ARGS__) a6, \
      SNAIL_ARG_(tn, 7, __VA_ARGS__) a7, SNAIL_ARG_(tn, 8, __VA_ARGS__) a8) \
      constness = 0

#define SNAIL_INTERFACE9_(tn, constness, ct, Method, ...)                   \
  virtual SNAIL_RESULT_(tn, __VA_ARGS__) ct Method(                         \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4, \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5, SNAIL_ARG_(tn, 6, __VA_ARGS__) a6, \
      SNAIL_ARG_(tn, 7, __VA_ARGS__) a7, SNAIL_ARG_(tn, 8, __VA_ARGS__) a8, \
      SNAIL_ARG_(tn, 9, __VA_ARGS__) gmock_a9) constness = 0

#define SNAIL_INTERFACE10_(tn, constness, ct, Method, ...)                    \
  virtual SNAIL_RESULT_(tn, __VA_ARGS__) ct Method(                           \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2,   \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4,   \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5, SNAIL_ARG_(tn, 6, __VA_ARGS__) a6,   \
      SNAIL_ARG_(tn, 7, __VA_ARGS__) a7, SNAIL_ARG_(tn, 8, __VA_ARGS__) a8,   \
      SNAIL_ARG_(tn, 9, __VA_ARGS__) a9, SNAIL_ARG_(tn, 10, __VA_ARGS__) a10) \
      constness = 0

#endif  // INCLUDE_TEST_INTERFACE_PURE_VIRTUAL_H_
