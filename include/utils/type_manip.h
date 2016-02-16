// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TYPE_MANIP_H_
#define TYPE_MANIP_H_

namespace utils {

////////////////////////////////////////////////////////////////////////////////
// class template Type2Type
// Converts each type into a unique, insipid type
// Invocation Type2Type<T> where T is a type
// Defines the type OriginalType which maps back to T
////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct Type2Type {
  typedef T OriginalType;
};

}  // namespace utils

#endif  // TYPE_MANIP_H_
