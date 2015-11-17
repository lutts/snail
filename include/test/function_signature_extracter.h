// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_TEST_FUNCTION_SIGNATURE_EXTRACTER_H_
#define INCLUDE_TEST_FUNCTION_SIGNATURE_EXTRACTER_H_

#include <tuple>

// Template struct FunctionSignatureExtracter<F>, where F must be a function
// type, contains the following typedefs:
//
//   Result:               the function's return type.
//   ArgumentN:            the type of the N-th argument, where N starts with 1.
//   ArgumentTuple:        the tuple type consisting of all parameters of F.
template <typename F>
struct FunctionSignatureExtracter;

template <typename R>
struct FunctionSignatureExtracter<R()> {
  typedef R Result;
  typedef ::std::tuple<> ArgumentTuple;
};

template <typename R, typename A1>
struct FunctionSignatureExtracter<R(A1)> : FunctionSignatureExtracter<R()> {
  typedef A1 Argument1;
  typedef ::std::tuple<A1> ArgumentTuple;
};

template <typename R, typename A1, typename A2>
struct FunctionSignatureExtracter<R(A1, A2)>
    : FunctionSignatureExtracter<R(A1)> {
  typedef A2 Argument2;
  typedef ::std::tuple<A1, A2> ArgumentTuple;
};

template <typename R, typename A1, typename A2, typename A3>
struct FunctionSignatureExtracter<R(A1, A2, A3)>
    : FunctionSignatureExtracter<R(A1, A2)> {
  typedef A3 Argument3;
  typedef ::std::tuple<A1, A2, A3> ArgumentTuple;
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
struct FunctionSignatureExtracter<R(A1, A2, A3, A4)>
    : FunctionSignatureExtracter<R(A1, A2, A3)> {
  typedef A4 Argument4;
  typedef ::std::tuple<A1, A2, A3, A4> ArgumentTuple;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
          typename A5>
struct FunctionSignatureExtracter<R(A1, A2, A3, A4, A5)>
    : FunctionSignatureExtracter<R(A1, A2, A3, A4)> {
  typedef A5 Argument5;
  typedef ::std::tuple<A1, A2, A3, A4, A5> ArgumentTuple;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
          typename A5, typename A6>
struct FunctionSignatureExtracter<R(A1, A2, A3, A4, A5, A6)>
    : FunctionSignatureExtracter<R(A1, A2, A3, A4, A5)> {
  typedef A6 Argument6;
  typedef ::std::tuple<A1, A2, A3, A4, A5, A6> ArgumentTuple;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
          typename A5, typename A6, typename A7>
struct FunctionSignatureExtracter<R(A1, A2, A3, A4, A5, A6, A7)>
    : FunctionSignatureExtracter<R(A1, A2, A3, A4, A5, A6)> {
  typedef A7 Argument7;
  typedef ::std::tuple<A1, A2, A3, A4, A5, A6, A7> ArgumentTuple;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
          typename A5, typename A6, typename A7, typename A8>
struct FunctionSignatureExtracter<R(A1, A2, A3, A4, A5, A6, A7, A8)>
    : FunctionSignatureExtracter<R(A1, A2, A3, A4, A5, A6, A7)> {
  typedef A8 Argument8;
  typedef ::std::tuple<A1, A2, A3, A4, A5, A6, A7, A8> ArgumentTuple;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
          typename A5, typename A6, typename A7, typename A8, typename A9>
struct FunctionSignatureExtracter<R(A1, A2, A3, A4, A5, A6, A7, A8, A9)>
    : FunctionSignatureExtracter<R(A1, A2, A3, A4, A5, A6, A7, A8)> {
  typedef A9 Argument9;
  typedef ::std::tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> ArgumentTuple;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
          typename A5, typename A6, typename A7, typename A8, typename A9,
          typename A10>
struct FunctionSignatureExtracter<R(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)>
    : FunctionSignatureExtracter<R(A1, A2, A3, A4, A5, A6, A7, A8, A9)> {
  typedef A10 Argument10;
  typedef ::std::tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10> ArgumentTuple;
};

#define SNAIL_RESULT_(tn, ...) \
  tn FunctionSignatureExtracter<__VA_ARGS__>::Result

// The type of argument N of the given function type.
#define SNAIL_ARG_(tn, N, ...) \
  tn FunctionSignatureExtracter<__VA_ARGS__>::Argument##N

#endif  // INCLUDE_TEST_FUNCTION_SIGNATURE_EXTRACTER_H_
