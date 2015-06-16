// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_I_TRACKABLE_H_
#define INCLUDE_UTILS_I_TRACKABLE_H_

namespace utils {

// nothing but says you are trackable, used by signal-slot mechanism
class ITrackable {
 public:
  ~ITrackable() = default;
};

}  // namespace utils

#endif  // INCLUDE_UTILS_I_TRACKABLE_H_
