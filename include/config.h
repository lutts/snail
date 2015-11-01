// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CONFIG_H_
#define INCLUDE_CONFIG_H_

#define FTO_HEADER(path, name) <path/fto_##name.h>
#define FTO_INTERFACE(c) fto::c

#ifndef DISABLE_TEST_CODE

#define FTO_BEGIN_NAMESPACE namespace fto {
#define FTO_END_NAMESPACE }

#define TEST_PROXY(RealClass) fto::RealClass##TestProxy

#define TEST_ONLY_MOVE_ASSIGNMENT(Cls)          \
  fto::Cls& operator=(fto::Cls&& rhs) {         \
    Cls& data = static_cast<Cls&>(rhs);         \
    swap(data);                                 \
    return *this;                               \
  }

#else  // DISABLE_TEST_CODE

#define FTO_BEGIN_NAMESPACE
#define FTO_END_NAMESPACE

#define TEST_PROXY(RealClass) RealClass
#define TEST_ONLY_MOVE_ASSIGNMENT(Cls)

#endif


#endif  // INCLUDE_CONFIG_H_
