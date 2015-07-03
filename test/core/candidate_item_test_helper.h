// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_CORE_CANDIDATE_ITEM_TEST_HELPER_H_
#define TEST_CORE_CANDIDATE_ITEM_TEST_HELPER_H_

#include <memory>

#include "utils/basic_utils.h"
#include "snail/candidate_item.h"

namespace snailcore {
namespace tests {

class CandidateItemTestHelper {
 public:
  static constexpr int DEFAULT_NUM_LEVEL = 2;
  static constexpr int DEFAULT_NUM_ITEMS = 3;

  static CandidateItem* genCandidateItemTree(
      int num_level = DEFAULT_NUM_LEVEL,
      int num_items = DEFAULT_NUM_ITEMS) {
    auto root_item = new CandidateItem();

    genCandidateItemSubTree(root_item, num_level, num_items);

    return root_item;
  }

 private:
  static void genCandidateItemSubTree(
      CandidateItem* parent, int num_level, int num_items) {
    --num_level;
    for (int i = 0; i < num_items; ++i) {
      auto item = new CandidateItem(
          xtestutils::genRandomString(),  // text
          xtestutils::genRandomString(),  // description
          parent);
      if (num_level) {
        genCandidateItemSubTree(item, num_level, num_items);
      }
    }
  }
};

}  // namespace tests
}  // namespace snailcore

#endif  // TEST_CORE_CANDIDATE_ITEM_TEST_HELPER_H_
