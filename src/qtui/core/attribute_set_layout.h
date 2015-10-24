// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CORE_ATTRIBUTE_SET_LAYOUT_H_
#define SRC_QTUI_CORE_ATTRIBUTE_SET_LAYOUT_H_

#include <vector>

#include "include/config.h"
#include FTO_HEADER(qtui, attribute_set_layout)
#include "utils/basic_utils.h"
#include "utils/u8string.h"
#include "utils/signal_slot_impl.h"

class QGridLayout;
class QTimer;

class AttributeSetLayoutImpl;

#define SNAIL_SIGSLOT2_IMPL_(sigName)                                   \
  sigName##SignalType sigName;                                          \
                                                                        \
  void when##sigName(                                                   \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject) {                 \
    sigName##SignalType::slot_type subscriber(handler);                 \
        if (trackObject)                                                \
          sigName.connect(subscriber.track_foreign(trackObject));       \
        else                                                            \
          sigName.connect(subscriber);                                  \
  }                                                                     \
  void cleanup##sigName##Slots() {                                      \
    sigName.num_slots();                                                \
  }

#define SNAIL_SIGSLOT2_IMPL(sigName, ...)                               \
  using sigName##Signature = __VA_ARGS__;                               \
  using sigName##SlotType = std::function<sigName##Signature>;          \
  using sigName##SignalType = boost::signals2::signal<sigName##Signature>; \
  SNAIL_SIGSLOT2_IMPL_(sigName)                                          \

class AttributeSetLayout
    : public FTO_INTERFACE(AttributeSetLayout)
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
      bool edit_mode);

  QGridLayout* layout() const;

 public:
  SNAIL_SIGSLOT2_IMPL(CreateAttrEditor,
                      IAttributeEditorView*(snailcore::IAttribute* attr));
  SNAIL_SIGSLOT2_IMPL(CloseAttributeEditors, void());

 private:
  SNAIL_DISABLE_COPY(AttributeSetLayout);

  std::shared_ptr<AttributeSetLayout> getSharedPtr() {
    return shared_from_this();
  }

  std::unique_ptr<AttributeSetLayoutImpl> impl;
  friend class AttributeSetLayoutImpl;
};


#endif  // SRC_QTUI_CORE_ATTRIBUTE_SET_LAYOUT_H_
