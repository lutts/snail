// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef ATTRIBUTE_COLLECTION_VIEW_H_
#define ATTRIBUTE_COLLECTION_VIEW_H_

#include <QObject>

#include "qtui/i_attribute_collection_view.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

namespace QSint {
class ActionGroup;
}  // namespace QSint

class QTableView;

class AttributeCollectionView : public IAttributeCollectionView
                              , public QObject {
 public:
  AttributeCollectionView();
  virtual ~AttributeCollectionView() = default;

  QWidget* getWidget() override;

  void setQModel(IAttributeCollectionQModel* qmodel) override;
  void setAttributeDelegate(IAttributeDelegate* attr_delegate) override;
  void openAttributeEditor(int row) override;
  void closeAttributeEditor(int row) override;

  void switchToDisplayMode() override;
  void switchToEditMode() override;
  void setSwitchModelButtonEnabled(bool enabled) override;

  QSint::ActionGroup* action_group_;
  QTableView* attr_table_view_;

 private:
  SNAIL_DISABLE_COPY(AttributeCollectionView);

  SNAIL_SIGSLOT_IMPL(UserSwitchMode);
  SNAIL_SIGSLOT_IMPL(UserMayClickAddAttribute);
};


#endif  // ATTRIBUTE_COLLECTION_VIEW_H_
