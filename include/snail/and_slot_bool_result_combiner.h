// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_AND_SLOT_BOOL_RESULT_COMBINER_H_
#define INCLUDE_SNAIL_AND_SLOT_BOOL_RESULT_COMBINER_H_

// and slot bool result, stop on first false
struct and_slot_bool_result_combiner {
  using result_type = bool;

  template<typename InputIterator>
  result_type operator()(InputIterator first, InputIterator last) const {
    // if no one interest in RequestClose event. return true
    if (first == last) return true;

    result_type and_result = *first++;
    if (!and_result)
      return and_result;

    while (first != last) {
      and_result &= *first;

      if (!and_result)
        return and_result;

      ++first;
    }

    return and_result;
  }
};

#endif  // INCLUDE_SNAIL_AND_SLOT_BOOL_RESULT_COMBINER_H_
