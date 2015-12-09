// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CONFIG_H_
#define INCLUDE_CONFIG_H_

#include <boost/polymorphic_cast.hpp>

/** Why we need a fto namespace when there's already a tests namespace?
 *
 *  Because we have decided that some_namespace::fto:XXX and its impl
 *  some_namespace::XXX has should have the same name,
 *  so if we not use "fto" but use "tests", we could not distinguish XXX
 *  in some_namespace::tests
 *
 *  Note that this also means we should not import fto namespace in mock_xxx.h
 *  because tests namespace should not include fto namespace symbols, tests
 *  should only use fto, but not include
 *
 *  fto namespace should always explicitly specified, eg. fto:XXX, fto::YYY
 */

// clang-format off
#define QUOTEME(x) QUOTEME_(x)
#define QUOTEME_(x) #x
#define FTO_HEADER(prefix, name) QUOTEME(prefix/fto_##name.h)
// clang-format on

#define FTO_NAMESPACE fto

#ifndef DISABLE_TEST_CODE

#define FTO_BEGIN_NAMESPACE namespace fto {
#define FTO_END_NAMESPACE }

#define TEST_PROXY(RealClass) fto::RealClass##TestProxy

#define TEST_ONLY_COPY_CONSTRUCT(Cls) \
  fto::Cls* clone() const { return new Cls(*this); }

#define TEST_ONLY_COPY_ASSIGNMENT(Cls)                                   \
  fto::Cls& operator=(const fto::Cls& rhs) {                             \
    const Cls* real_rhs = boost::polymorphic_downcast<const Cls*>(&rhs); \
    return this->operator=(*real_rhs);                                   \
  }

#define TEST_ONLY_MOVE_CONSTRUCT(Cls) \
  fto::Cls* moveClone() { return new Cls(std::move(*this)); }

#define TEST_ONLY_MOVE_ASSIGNMENT(Cls)                       \
  fto::Cls& operator=(fto::Cls&& rhs) { /* NOLINT*/          \
    Cls* real_rhs = boost::polymorphic_downcast<Cls*>(&rhs); \
    return this->operator=(std::move(*real_rhs));            \
  }

#define TEST_ONLY_SWAP(Cls)                                  \
  void swap_with(fto::Cls& rhs) {                            \
    Cls* real_rhs = boost::polymorphic_downcast<Cls*>(&rhs); \
    return this->swap(*real_rhs);                            \
  }

#else  // DISABLE_TEST_CODE

#define FTO_BEGIN_NAMESPACE
#define FTO_END_NAMESPACE

#define TEST_PROXY(RealClass) RealClass
#define TEST_ONLY_COPY_CONSTRUCT(Cls)
#define TEST_ONLY_COPY_ASSIGNMENT(Cls)
#define TEST_ONLY_MOVE_CONSTRUCT(Cls)
#define TEST_ONLY_MOVE_ASSIGNMENT(Cls)
#define TEST_ONLY_SWAP(Cls)

#endif

#endif  // INCLUDE_CONFIG_H_
