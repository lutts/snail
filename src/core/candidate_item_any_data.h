// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_CANDIDATE_ITEM_ANY_DATA_H_
#define SRC_CORE_CANDIDATE_ITEM_ANY_DATA_H_

#include <boost/any.hpp>

#include "snail/candidate_item.h"
#include "utils/basic_utils.h"

namespace snailcore {

class CandidateItemAnyData : public CandidateItem {
 public:
  void set_data(const boost::any& data) {
    data_ = data;
  }

  template <typename DataType>
  DataType data() const {
    return boost::any_cast<DataType>(data_);
  }

 private:
  boost::any data_;
};

}  // namespace snailcore


#endif  // SRC_CORE_CANDIDATE_ITEM_ANY_DATA_H_
