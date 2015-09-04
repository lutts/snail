// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef COMMON_H_
#define COMMON_H_

#if defined(Q_OS_WIN)
#  if !defined(QT_QTPROPERTYBROWSER_EXPORT) && !defined(QT_QTPROPERTYBROWSER_IMPORT)
#    define QT_QTPROPERTYBROWSER_EXPORT
#  elif defined(QT_QTPROPERTYBROWSER_IMPORT)
#    if defined(QT_QTPROPERTYBROWSER_EXPORT)
#      undef QT_QTPROPERTYBROWSER_EXPORT
#    endif
#    define QT_QTPROPERTYBROWSER_EXPORT __declspec(dllimport)
#  elif defined(QT_QTPROPERTYBROWSER_EXPORT)
#    undef QT_QTPROPERTYBROWSER_EXPORT
#    define QT_QTPROPERTYBROWSER_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTPROPERTYBROWSER_EXPORT
#endif

#endif  // COMMON_H_
