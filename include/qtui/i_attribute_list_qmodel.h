// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_ATTRIBUTE_LIST_QMODEL_H_
#define I_ATTRIBUTE_LIST_QMODEL_H_

#include <vector>

namespace snailcore {
class IAttribute;
}  // namespace snailcore

class IAttributeListQModel {
 public:
  virtual ~IAttributeListQModel() = default;

  virtual void setAttributeList(
      std::vector<snailcore::IAttribute*> attr_list) = 0;
};

#endif  // I_ATTRIBUTE_LIST_QMODEL_H_
