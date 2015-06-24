// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_ATTRIBUTE_LIST_QMODEL_H_
#define INCLUDE_QTUI_I_ATTRIBUTE_LIST_QMODEL_H_

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

#endif  // INCLUDE_QTUI_I_ATTRIBUTE_LIST_QMODEL_H_
