// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#undef SNAIL_INTERFACE0
#undef SNAIL_INTERFACE1
#undef SNAIL_INTERFACE2
#undef SNAIL_INTERFACE3
#undef SNAIL_INTERFACE4
#undef SNAIL_INTERFACE5
#undef SNAIL_INTERFACE6
#undef SNAIL_INTERFACE7
#undef SNAIL_INTERFACE8
#undef SNAIL_INTERFACE9
#undef SNAIL_INTERFACE10

#undef SNAIL_CONST_INTERFACE0
#undef SNAIL_CONST_INTERFACE1
#undef SNAIL_CONST_INTERFACE2
#undef SNAIL_CONST_INTERFACE3
#undef SNAIL_CONST_INTERFACE4
#undef SNAIL_CONST_INTERFACE5
#undef SNAIL_CONST_INTERFACE6
#undef SNAIL_CONST_INTERFACE7
#undef SNAIL_CONST_INTERFACE8
#undef SNAIL_CONST_INTERFACE9
#undef SNAIL_CONST_INTERFACE10

#undef SNAIL_INTERFACE0_T
#undef SNAIL_INTERFACE1_T
#undef SNAIL_INTERFACE2_T
#undef SNAIL_INTERFACE3_T
#undef SNAIL_INTERFACE4_T
#undef SNAIL_INTERFACE5_T
#undef SNAIL_INTERFACE6_T
#undef SNAIL_INTERFACE7_T
#undef SNAIL_INTERFACE8_T
#undef SNAIL_INTERFACE9_T
#undef SNAIL_INTERFACE10_T

#undef SNAIL_CONST_INTERFACE0_T
#undef SNAIL_CONST_INTERFACE1_T
#undef SNAIL_CONST_INTERFACE2_T
#undef SNAIL_CONST_INTERFACE3_T
#undef SNAIL_CONST_INTERFACE4_T
#undef SNAIL_CONST_INTERFACE5_T
#undef SNAIL_CONST_INTERFACE6_T
#undef SNAIL_CONST_INTERFACE7_T
#undef SNAIL_CONST_INTERFACE8_T
#undef SNAIL_CONST_INTERFACE9_T
#undef SNAIL_CONST_INTERFACE10_T

#undef SNAIL_INTERFACE0_WITH_CALLTYPE
#undef SNAIL_INTERFACE1_WITH_CALLTYPE
#undef SNAIL_INTERFACE2_WITH_CALLTYPE
#undef SNAIL_INTERFACE3_WITH_CALLTYPE
#undef SNAIL_INTERFACE4_WITH_CALLTYPE
#undef SNAIL_INTERFACE5_WITH_CALLTYPE
#undef SNAIL_INTERFACE6_WITH_CALLTYPE
#undef SNAIL_INTERFACE7_WITH_CALLTYPE
#undef SNAIL_INTERFACE8_WITH_CALLTYPE
#undef SNAIL_INTERFACE9_WITH_CALLTYPE
#undef SNAIL_INTERFACE10_WITH_CALLTYPE

#undef SNAIL_CONST_INTERFACE0_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE1_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE2_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE3_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE4_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE5_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE6_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE7_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE8_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE9_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE10_WITH_CALLTYPE

#undef SNAIL_INTERFACE0_T_WITH_CALLTYPE
#undef SNAIL_INTERFACE1_T_WITH_CALLTYPE
#undef SNAIL_INTERFACE2_T_WITH_CALLTYPE
#undef SNAIL_INTERFACE3_T_WITH_CALLTYPE
#undef SNAIL_INTERFACE4_T_WITH_CALLTYPE
#undef SNAIL_INTERFACE5_T_WITH_CALLTYPE
#undef SNAIL_INTERFACE6_T_WITH_CALLTYPE
#undef SNAIL_INTERFACE7_T_WITH_CALLTYPE
#undef SNAIL_INTERFACE8_T_WITH_CALLTYPE
#undef SNAIL_INTERFACE9_T_WITH_CALLTYPE
#undef SNAIL_INTERFACE10_T_WITH_CALLTYPE

#undef SNAIL_CONST_INTERFACE0_T_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE1_T_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE2_T_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE3_T_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE4_T_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE5_T_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE6_T_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE7_T_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE8_T_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE9_T_WITH_CALLTYPE
#undef SNAIL_CONST_INTERFACE10_T_WITH_CALLTYPE

#ifdef INTERFACE_DEFINITION_PHASE

#include "test/interface_pure_virtual.h"

#define SNAIL_INTERFACE0(m, ...) SNAIL_INTERFACE0_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE1(m, ...) SNAIL_INTERFACE1_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE2(m, ...) SNAIL_INTERFACE2_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE3(m, ...) SNAIL_INTERFACE3_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE4(m, ...) SNAIL_INTERFACE4_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE5(m, ...) SNAIL_INTERFACE5_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE6(m, ...) SNAIL_INTERFACE6_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE7(m, ...) SNAIL_INTERFACE7_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE8(m, ...) SNAIL_INTERFACE8_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE9(m, ...) SNAIL_INTERFACE9_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE10(m, ...) SNAIL_INTERFACE10_(, , , m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0(m, ...) SNAIL_INTERFACE0_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1(m, ...) SNAIL_INTERFACE1_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2(m, ...) SNAIL_INTERFACE2_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3(m, ...) SNAIL_INTERFACE3_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4(m, ...) SNAIL_INTERFACE4_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5(m, ...) SNAIL_INTERFACE5_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6(m, ...) SNAIL_INTERFACE6_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7(m, ...) SNAIL_INTERFACE7_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8(m, ...) SNAIL_INTERFACE8_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9(m, ...) SNAIL_INTERFACE9_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10(m, ...) SNAIL_INTERFACE10_(, const, , m, __VA_ARGS__)

#define SNAIL_INTERFACE0_T(m, ...) SNAIL_INTERFACE0_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE1_T(m, ...) SNAIL_INTERFACE1_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE2_T(m, ...) SNAIL_INTERFACE2_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE3_T(m, ...) SNAIL_INTERFACE3_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE4_T(m, ...) SNAIL_INTERFACE4_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE5_T(m, ...) SNAIL_INTERFACE5_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE6_T(m, ...) SNAIL_INTERFACE6_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE7_T(m, ...) SNAIL_INTERFACE7_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE8_T(m, ...) SNAIL_INTERFACE8_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE9_T(m, ...) SNAIL_INTERFACE9_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE10_T(m, ...) SNAIL_INTERFACE10_(typename, , , m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0_T(m, ...)                \
  SNAIL_INTERFACE0_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1_T(m, ...)                \
  SNAIL_INTERFACE1_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2_T(m, ...)                \
  SNAIL_INTERFACE2_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3_T(m, ...)                \
  SNAIL_INTERFACE3_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4_T(m, ...)                \
  SNAIL_INTERFACE4_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5_T(m, ...)                \
  SNAIL_INTERFACE5_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6_T(m, ...)                \
  SNAIL_INTERFACE6_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7_T(m, ...)                \
  SNAIL_INTERFACE7_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8_T(m, ...)                \
  SNAIL_INTERFACE8_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9_T(m, ...)                \
  SNAIL_INTERFACE9_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10_T(m, ...)               \
  SNAIL_INTERFACE10_(typename, const, , m, __VA_ARGS__)

#define SNAIL_INTERFACE0_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE0_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE1_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE1_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE2_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE2_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE3_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE3_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE4_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE4_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE5_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE5_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE6_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE6_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE7_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE7_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE8_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE8_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE9_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE9_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE10_WITH_CALLTYPE(ct, m, ...)     \
  SNAIL_INTERFACE10_(, , ct, m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_INTERFACE0_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_INTERFACE1_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_INTERFACE2_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_INTERFACE3_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_INTERFACE4_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_INTERFACE5_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_INTERFACE6_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_INTERFACE7_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_INTERFACE8_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_INTERFACE9_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10_WITH_CALLTYPE(ct, m, ...)       \
  SNAIL_INTERFACE10_(, const, ct, m, __VA_ARGS__)

#define SNAIL_INTERFACE0_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_INTERFACE0_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE1_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_INTERFACE1_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE2_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_INTERFACE2_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE3_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_INTERFACE3_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE4_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_INTERFACE4_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE5_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_INTERFACE5_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE6_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_INTERFACE6_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE7_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_INTERFACE7_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE8_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_INTERFACE8_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE9_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_INTERFACE9_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE10_T_WITH_CALLTYPE(ct, m, ...)   \
  SNAIL_INTERFACE10_(typename, , ct, m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE0_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE1_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE2_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE3_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE4_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE5_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE6_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE7_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE8_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_INTERFACE9_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10_T_WITH_CALLTYPE(ct, m, ...)     \
  SNAIL_INTERFACE10_(typename, const, ct, m, __VA_ARGS__)

#elif defined(INTERFACE_MOCK_PHASE)

#define SNAIL_INTERFACE0(m, ...) MOCK_METHOD0(m, __VA_ARGS__)
#define SNAIL_INTERFACE1(m, ...) MOCK_METHOD1(m, __VA_ARGS__)
#define SNAIL_INTERFACE2(m, ...) MOCK_METHOD2(m, __VA_ARGS__)
#define SNAIL_INTERFACE3(m, ...) MOCK_METHOD3(m, __VA_ARGS__)
#define SNAIL_INTERFACE4(m, ...) MOCK_METHOD4(m, __VA_ARGS__)
#define SNAIL_INTERFACE5(m, ...) MOCK_METHOD5(m, __VA_ARGS__)
#define SNAIL_INTERFACE6(m, ...) MOCK_METHOD6(m, __VA_ARGS__)
#define SNAIL_INTERFACE7(m, ...) MOCK_METHOD7(m, __VA_ARGS__)
#define SNAIL_INTERFACE8(m, ...) MOCK_METHOD8(m, __VA_ARGS__)
#define SNAIL_INTERFACE9(m, ...) MOCK_METHOD9(m, __VA_ARGS__)
#define SNAIL_INTERFACE10(m, ...) MOCK_METHOD10(m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0(m, ...) MOCK_CONST_METHOD0(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1(m, ...) MOCK_CONST_METHOD1(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2(m, ...) MOCK_CONST_METHOD2(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3(m, ...) MOCK_CONST_METHOD3(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4(m, ...) MOCK_CONST_METHOD4(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5(m, ...) MOCK_CONST_METHOD5(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6(m, ...) MOCK_CONST_METHOD6(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7(m, ...) MOCK_CONST_METHOD7(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8(m, ...) MOCK_CONST_METHOD8(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9(m, ...) MOCK_CONST_METHOD9(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10(m, ...) MOCK_CONST_METHOD10(m, __VA_ARGS__)

#define SNAIL_INTERFACE0_T(m, ...) MOCK_METHOD0_T(m, __VA_ARGS__)
#define SNAIL_INTERFACE1_T(m, ...) MOCK_METHOD1_T(m, __VA_ARGS__)
#define SNAIL_INTERFACE2_T(m, ...) MOCK_METHOD2_T(m, __VA_ARGS__)
#define SNAIL_INTERFACE3_T(m, ...) MOCK_METHOD3_T(m, __VA_ARGS__)
#define SNAIL_INTERFACE4_T(m, ...) MOCK_METHOD4_T(m, __VA_ARGS__)
#define SNAIL_INTERFACE5_T(m, ...) MOCK_METHOD5_T(m, __VA_ARGS__)
#define SNAIL_INTERFACE6_T(m, ...) MOCK_METHOD6_T(m, __VA_ARGS__)
#define SNAIL_INTERFACE7_T(m, ...) MOCK_METHOD7_T(m, __VA_ARGS__)
#define SNAIL_INTERFACE8_T(m, ...) MOCK_METHOD8_T(m, __VA_ARGS__)
#define SNAIL_INTERFACE9_T(m, ...) MOCK_METHOD9_T(m, __VA_ARGS__)
#define SNAIL_INTERFACE10_T(m, ...) MOCK_METHOD10_T(m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0_T(m, ...) MOCK_CONST_METHOD0_T(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1_T(m, ...) MOCK_CONST_METHOD1_T(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2_T(m, ...) MOCK_CONST_METHOD2_T(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3_T(m, ...) MOCK_CONST_METHOD3_T(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4_T(m, ...) MOCK_CONST_METHOD4_T(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5_T(m, ...) MOCK_CONST_METHOD5_T(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6_T(m, ...) MOCK_CONST_METHOD6_T(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7_T(m, ...) MOCK_CONST_METHOD7_T(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8_T(m, ...) MOCK_CONST_METHOD8_T(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9_T(m, ...) MOCK_CONST_METHOD9_T(m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10_T(m, ...) MOCK_CONST_METHOD10_T(m, __VA_ARGS__)

#define SNAIL_INTERFACE0_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_METHOD0_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE1_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_METHOD1_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE2_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_METHOD2_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE3_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_METHOD3_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE4_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_METHOD4_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE5_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_METHOD5_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE6_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_METHOD6_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE7_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_METHOD7_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE8_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_METHOD8_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE9_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_METHOD9_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE10_WITH_CALLTYPE(ct, m, ...)     \
  MOCK_METHOD10_WITH_CALLTYPE(ct, m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0_WITH_CALLTYPE(ct, m, ...)        \
  MOCK_CONST_METHOD0_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1_WITH_CALLTYPE(ct, m, ...)        \
  MOCK_CONST_METHOD1_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2_WITH_CALLTYPE(ct, m, ...)        \
  MOCK_CONST_METHOD2_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3_WITH_CALLTYPE(ct, m, ...)        \
  MOCK_CONST_METHOD3_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4_WITH_CALLTYPE(ct, m, ...)        \
  MOCK_CONST_METHOD4_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5_WITH_CALLTYPE(ct, m, ...)        \
  MOCK_CONST_METHOD5_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6_WITH_CALLTYPE(ct, m, ...)        \
  MOCK_CONST_METHOD6_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7_WITH_CALLTYPE(ct, m, ...)        \
  MOCK_CONST_METHOD7_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8_WITH_CALLTYPE(ct, m, ...)        \
  MOCK_CONST_METHOD8_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9_WITH_CALLTYPE(ct, m, ...)        \
  MOCK_CONST_METHOD9_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10_WITH_CALLTYPE(ct, m, ...)       \
  MOCK_CONST_METHOD10_WITH_CALLTYPE(ct, m, __VA_ARGS__)

#define SNAIL_INTERFACE0_T_WITH_CALLTYPE(ct, m, ...)    \
  MOCK_METHOD0_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE1_T_WITH_CALLTYPE(ct, m, ...)    \
  MOCK_METHOD1_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE2_T_WITH_CALLTYPE(ct, m, ...)    \
  MOCK_METHOD2_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE3_T_WITH_CALLTYPE(ct, m, ...)    \
  MOCK_METHOD3_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE4_T_WITH_CALLTYPE(ct, m, ...)    \
  MOCK_METHOD4_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE5_T_WITH_CALLTYPE(ct, m, ...)    \
  MOCK_METHOD5_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE6_T_WITH_CALLTYPE(ct, m, ...)    \
  MOCK_METHOD6_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE7_T_WITH_CALLTYPE(ct, m, ...)    \
  MOCK_METHOD7_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE8_T_WITH_CALLTYPE(ct, m, ...)    \
  MOCK_METHOD8_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE9_T_WITH_CALLTYPE(ct, m, ...)    \
  MOCK_METHOD9_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE10_T_WITH_CALLTYPE(ct, m, ...)   \
  MOCK_METHOD10_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0_T_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_CONST_METHOD0_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1_T_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_CONST_METHOD1_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2_T_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_CONST_METHOD2_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3_T_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_CONST_METHOD3_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4_T_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_CONST_METHOD4_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5_T_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_CONST_METHOD5_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6_T_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_CONST_METHOD6_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7_T_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_CONST_METHOD7_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8_T_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_CONST_METHOD8_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9_T_WITH_CALLTYPE(ct, m, ...)      \
  MOCK_CONST_METHOD9_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10_T_WITH_CALLTYPE(ct, m, ...)     \
  MOCK_CONST_METHOD10_T_WITH_CALLTYPE(ct, m, __VA_ARGS__)

#elif defined(INTERFACE_TEST_PROXY_PHASE)

#include "test/interface_test_proxy.h"

#define SNAIL_INTERFACE0(m, ...) SNAIL_TEST_PROXY0_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE1(m, ...) SNAIL_TEST_PROXY1_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE2(m, ...) SNAIL_TEST_PROXY2_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE3(m, ...) SNAIL_TEST_PROXY3_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE4(m, ...) SNAIL_TEST_PROXY4_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE5(m, ...) SNAIL_TEST_PROXY5_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE6(m, ...) SNAIL_TEST_PROXY6_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE7(m, ...) SNAIL_TEST_PROXY7_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE8(m, ...) SNAIL_TEST_PROXY8_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE9(m, ...) SNAIL_TEST_PROXY9_(, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE10(m, ...) SNAIL_TEST_PROXY10_(, , , m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0(m, ...)          \
  SNAIL_TEST_PROXY0_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1(m, ...)          \
  SNAIL_TEST_PROXY1_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2(m, ...)          \
  SNAIL_TEST_PROXY2_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3(m, ...)          \
  SNAIL_TEST_PROXY3_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4(m, ...)          \
  SNAIL_TEST_PROXY4_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5(m, ...)          \
  SNAIL_TEST_PROXY5_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6(m, ...)          \
  SNAIL_TEST_PROXY6_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7(m, ...)          \
  SNAIL_TEST_PROXY7_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8(m, ...)          \
  SNAIL_TEST_PROXY8_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9(m, ...)          \
  SNAIL_TEST_PROXY9_(, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10(m, ...)                 \
  SNAIL_TEST_PROXY10_(, const, , m, __VA_ARGS__)

#define SNAIL_INTERFACE0_T(m, ...)                      \
  SNAIL_TEST_PROXY0_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE1_T(m, ...)                      \
  SNAIL_TEST_PROXY1_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE2_T(m, ...)                      \
  SNAIL_TEST_PROXY2_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE3_T(m, ...)                      \
  SNAIL_TEST_PROXY3_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE4_T(m, ...)                      \
  SNAIL_TEST_PROXY4_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE5_T(m, ...)                      \
  SNAIL_TEST_PROXY5_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE6_T(m, ...)                      \
  SNAIL_TEST_PROXY6_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE7_T(m, ...)                      \
  SNAIL_TEST_PROXY7_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE8_T(m, ...)                      \
  SNAIL_TEST_PROXY8_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE9_T(m, ...)                      \
  SNAIL_TEST_PROXY9_(typename, , , m, __VA_ARGS__)
#define SNAIL_INTERFACE10_T(m, ...)                     \
  SNAIL_TEST_PROXY10_(typename, , , m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0_T(m, ...)                \
  SNAIL_TEST_PROXY0_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1_T(m, ...)                \
  SNAIL_TEST_PROXY1_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2_T(m, ...)                \
  SNAIL_TEST_PROXY2_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3_T(m, ...)                \
  SNAIL_TEST_PROXY3_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4_T(m, ...)                \
  SNAIL_TEST_PROXY4_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5_T(m, ...)                \
  SNAIL_TEST_PROXY5_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6_T(m, ...)                \
  SNAIL_TEST_PROXY6_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7_T(m, ...)                \
  SNAIL_TEST_PROXY7_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8_T(m, ...)                \
  SNAIL_TEST_PROXY8_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9_T(m, ...)                \
  SNAIL_TEST_PROXY9_(typename, const, , m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10_T(m, ...)                       \
  SNAIL_TEST_PROXY10_(typename, const, , m, __VA_ARGS__)

#define SNAIL_INTERFACE0_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY0_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE1_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY1_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE2_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY2_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE3_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY3_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE4_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY4_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE5_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY5_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE6_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY6_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE7_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY7_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE8_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY8_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE9_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY9_(, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE10_WITH_CALLTYPE(ct, m, ...)     \
  SNAIL_TEST_PROXY10_(, , ct, m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_TEST_PROXY0_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_TEST_PROXY1_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_TEST_PROXY2_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_TEST_PROXY3_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_TEST_PROXY4_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_TEST_PROXY5_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_TEST_PROXY6_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_TEST_PROXY7_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_TEST_PROXY8_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9_WITH_CALLTYPE(ct, m, ...)        \
  SNAIL_TEST_PROXY9_(, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10_WITH_CALLTYPE(ct, m, ...)       \
  SNAIL_TEST_PROXY10_(, const, ct, m, __VA_ARGS__)

#define SNAIL_INTERFACE0_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_TEST_PROXY0_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE1_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_TEST_PROXY1_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE2_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_TEST_PROXY2_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE3_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_TEST_PROXY3_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE4_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_TEST_PROXY4_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE5_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_TEST_PROXY5_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE6_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_TEST_PROXY6_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE7_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_TEST_PROXY7_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE8_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_TEST_PROXY8_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE9_T_WITH_CALLTYPE(ct, m, ...)    \
  SNAIL_TEST_PROXY9_(typename, , ct, m, __VA_ARGS__)
#define SNAIL_INTERFACE10_T_WITH_CALLTYPE(ct, m, ...)   \
  SNAIL_TEST_PROXY10_(typename, , ct, m, __VA_ARGS__)

#define SNAIL_CONST_INTERFACE0_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY0_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE1_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY1_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE2_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY2_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE3_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY3_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE4_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY4_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE5_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY5_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE6_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY6_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE7_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY7_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE8_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY8_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE9_T_WITH_CALLTYPE(ct, m, ...)      \
  SNAIL_TEST_PROXY9_(typename, const, ct, m, __VA_ARGS__)
#define SNAIL_CONST_INTERFACE10_T_WITH_CALLTYPE(ct, m, ...)     \
  SNAIL_TEST_PROXY10_(typename, const, ct, m, __VA_ARGS__)

#endif  // INTERFACE_DEFINITION_PHASE
