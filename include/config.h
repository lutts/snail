// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CONFIG_H_
#define INCLUDE_CONFIG_H_

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

#define FTO_HEADER(path, name) <path/fto_##name.h>
#define FTO_NAMESPACE fto

#ifndef DISABLE_TEST_CODE

#define FTO_BEGIN_NAMESPACE namespace fto {
#define FTO_END_NAMESPACE }

#define TEST_PROXY(RealClass) fto::RealClass##TestProxy

#define TEST_ONLY_MOVE_ASSIGNMENT(Cls)              \
  fto::Cls& operator=(fto::Cls&& rhs) { /* NOLINT*/ \
    Cls& data = static_cast<Cls&>(rhs);             \
    this->swap(data);                               \
    return *this;                                   \
  }

#else  // DISABLE_TEST_CODE

#define FTO_BEGIN_NAMESPACE
#define FTO_END_NAMESPACE

#define TEST_PROXY(RealClass) RealClass
#define TEST_ONLY_MOVE_ASSIGNMENT(Cls)

#endif

#endif  // INCLUDE_CONFIG_H_
