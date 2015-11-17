// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_TEST_INTERFACE_TEST_PROXY_H_
#define INCLUDE_TEST_INTERFACE_TEST_PROXY_H_

#include "test/function_signature_extracter.h"

#define SNAIL_TEST_PROXY0_(tn, constness, ct, Method, ...) \
  SNAIL_RESULT_(tn, __VA_ARGS__) ct Method() constness {   \
    return self_->Method();                                \
  }

#define SNAIL_TEST_PROXY1_(tn, constness, ct, Method, ...) \
  SNAIL_RESULT_(tn, __VA_ARGS__)                           \
  ct Method(SNAIL_ARG_(tn, 1, __VA_ARGS__) a1) constness { \
    return self_->Method(a1);                              \
  }

#define SNAIL_TEST_PROXY2_(tn, constness, ct, Method, ...) \
  SNAIL_RESULT_(tn, __VA_ARGS__)                           \
  ct Method(SNAIL_ARG_(tn, 1, __VA_ARGS__) a1,             \
            SNAIL_ARG_(tn, 2, __VA_ARGS__) a2) constness { \
    return self_->Method(a1, a2);                          \
  }

#define SNAIL_TEST_PROXY3_(tn, constness, ct, Method, ...) \
  SNAIL_RESULT_(tn, __VA_ARGS__)                           \
  ct Method(SNAIL_ARG_(tn, 1, __VA_ARGS__) a1,             \
            SNAIL_ARG_(tn, 2, __VA_ARGS__) a2,             \
            SNAIL_ARG_(tn, 3, __VA_ARGS__) a3) constness { \
    return self_->Method(a1, a2, a3);                      \
  }

#define SNAIL_TEST_PROXY4_(tn, constness, ct, Method, ...)                  \
  SNAIL_RESULT_(tn, __VA_ARGS__)                                            \
  ct Method(                                                                \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4) \
      constness {                                                           \
    return self_->Method(a1, a2, a3, a4);                                   \
  }

#define SNAIL_TEST_PROXY5_(tn, constness, ct, Method, ...)                  \
  SNAIL_RESULT_(tn, __VA_ARGS__)                                            \
  ct Method(                                                                \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4, \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5) constness {                        \
    return self_->Method(a1, a2, a3, a4, a5);                               \
  }

#define SNAIL_TEST_PROXY6_(tn, constness, ct, Method, ...)                  \
  SNAIL_RESULT_(tn, __VA_ARGS__)                                            \
  ct Method(                                                                \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4, \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5, SNAIL_ARG_(tn, 6, __VA_ARGS__) a6) \
      constness {                                                           \
    return self_->Method(a1, a2, a3, a4, a5, a6);                           \
  }

#define SNAIL_TEST_PROXY7_(tn, constness, ct, Method, ...)                  \
  SNAIL_RESULT_(tn, __VA_ARGS__)                                            \
  ct Method(                                                                \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4, \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5, SNAIL_ARG_(tn, 6, __VA_ARGS__) a6, \
      SNAIL_ARG_(tn, 7, __VA_ARGS__) a7) constness {                        \
    return self_->Method(a1, a2, a3, a4, a5, a6, a7);                       \
  }

#define SNAIL_TEST_PROXY8_(tn, constness, ct, Method, ...)                  \
  SNAIL_RESULT_(tn, __VA_ARGS__)                                            \
  ct Method(                                                                \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4, \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5, SNAIL_ARG_(tn, 6, __VA_ARGS__) a6, \
      SNAIL_ARG_(tn, 7, __VA_ARGS__) a7, SNAIL_ARG_(tn, 8, __VA_ARGS__) a8) \
      constness {                                                           \
    return self_->Method(a1, a2, a3, a4, a5, a6, a7, a8);                   \
  }

#define SNAIL_TEST_PROXY9_(tn, constness, ct, Method, ...)                  \
  SNAIL_RESULT_(tn, __VA_ARGS__)                                            \
  ct Method(                                                                \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2, \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4, \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5, SNAIL_ARG_(tn, 6, __VA_ARGS__) a6, \
      SNAIL_ARG_(tn, 7, __VA_ARGS__) a7, SNAIL_ARG_(tn, 8, __VA_ARGS__) a8, \
      SNAIL_ARG_(tn, 9, __VA_ARGS__) gmock_a9) constness {                  \
    return self_->Method(a1, a2, a3, a4, a5, a6, a7, a8, a9);               \
  }

#define SNAIL_TEST_PROXY10_(tn, constness, ct, Method, ...)                   \
  SNAIL_RESULT_(tn, __VA_ARGS__)                                              \
  ct Method(                                                                  \
      SNAIL_ARG_(tn, 1, __VA_ARGS__) a1, SNAIL_ARG_(tn, 2, __VA_ARGS__) a2,   \
      SNAIL_ARG_(tn, 3, __VA_ARGS__) a3, SNAIL_ARG_(tn, 4, __VA_ARGS__) a4,   \
      SNAIL_ARG_(tn, 5, __VA_ARGS__) a5, SNAIL_ARG_(tn, 6, __VA_ARGS__) a6,   \
      SNAIL_ARG_(tn, 7, __VA_ARGS__) a7, SNAIL_ARG_(tn, 8, __VA_ARGS__) a8,   \
      SNAIL_ARG_(tn, 9, __VA_ARGS__) a9, SNAIL_ARG_(tn, 10, __VA_ARGS__) a10) \
      constness {                                                             \
    return self_->Method(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);            \
  }

#endif  // INCLUDE_TEST_INTERFACE_TEST_PROXY_H_
