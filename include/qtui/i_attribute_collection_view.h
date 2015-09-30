// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_ATTRIBUTE_COLLECTION_VIEW_H_
#define INCLUDE_QTUI_I_ATTRIBUTE_COLLECTION_VIEW_H_

#include "src/qtui/generic_view_base.h"
#include "utils/signal_slot.h"

class IAttributeCollectionQModel;
class IAttributeDelegate;

class IAttributeCollectionView : public GenericViewBase {
 public:
  virtual ~IAttributeCollectionView() = default;

  SNAIL_SIGSLOT2(UserSwitchMode, void());
  SNAIL_SIGSLOT2(UserMayClickAddAttribute, void(int row));

  virtual void setQModel(IAttributeCollectionQModel* qmodel) = 0;
  virtual void setAttributeDelegate(IAttributeDelegate* attr_delegate) = 0;
  virtual void openAttributeEditor(int row) = 0;
  virtual void closeAttributeEditor(int row) = 0;

  virtual void switchToDisplayMode() = 0;
  virtual void switchToEditMode() = 0;
  virtual void setSwitchModelButtonEnabled(bool enabled) = 0;
};


#endif  // INCLUDE_QTUI_I_ATTRIBUTE_COLLECTION_VIEW_H_
