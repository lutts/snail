// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CORE_ATTRIBUTE_SET_LAYOUT_H_
#define SRC_QTUI_CORE_ATTRIBUTE_SET_LAYOUT_H_

#include "qtui/i_attribute_set_layout.h"
#include "utils/basic_utils.h"
#include "utils/u8string.h"
#include "utils/signal_slot_impl.h"

class QGridLayout;
class QTimer;

class AttributeSetLayoutImpl;

class AttributeSetLayout
    : public IAttributeSetLayout
    , public utils::ITrackable
    , public std::enable_shared_from_this<AttributeSetLayout> {
 public:
  enum {
    kNameColumn = 0,
    kValueColumn = 1,
  };

  AttributeSetLayout();
  virtual ~AttributeSetLayout();

  void setAttributeSuppliers(
      const std::vector<snailcore::IAttributeSupplier*>& attr_suppliers,
      bool edit_mode) override;

  QGridLayout* layout() const;

 private:
  SNAIL_DISABLE_COPY(AttributeSetLayout);

  SNAIL_SIGSLOT_IMPL(CreateAttrEditor);
  SNAIL_SIGSLOT_IMPL(CloseAttributeEditors);

  std::shared_ptr<AttributeSetLayout> getSharedPtr() {
    return shared_from_this();
  }

  std::unique_ptr<AttributeSetLayoutImpl> impl;
  friend class AttributeSetLayoutImpl;
};


#endif  // SRC_QTUI_CORE_ATTRIBUTE_SET_LAYOUT_H_
