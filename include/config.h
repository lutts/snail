// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef CONFIG_H_
#define CONFIG_H_

#define FTO_HEADER(path, name) <path/fto_##name.h>
#define FTO_INTERFACE(c) fto::c

#define FTO_BEGIN_NAMESPACE namespace fto {
#define FTO_END_NAMESPACE }

#ifndef DISABLE_TEST_CODE

#define OPT_FTO_BEGIN_NAMESPACE namespace fto {
#define OPT_FTO_END_NAMESPACE }

#else  // DISABLE_TEST_CODE

#define OPT_FTO_BEGIN_NAMESPACE
#define OPT_FTO_END_NAMESPACE

#endif


#endif  // CONFIG_H_
