// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_I18N_H_
#define INCLUDE_UTILS_I18N_H_

#include <libintl.h>

#define _(String) dgettext(PACKAGE, String)
#ifdef gettext_noop
#define N_(String) gettext_noop(String)
#else
#define N_(String) (String)
#endif

#endif  // INCLUDE_UTILS_I18N_H_
