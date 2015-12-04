// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_GENERIC_ATTRIBUTE_SUPPLIER_H_
#define SRC_CORE_GENERIC_ATTRIBUTE_SUPPLIER_H_

#include <vector>
#include <memory>

#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"
#include "snail/i_attribute_supplier.h"

namespace snailcore {

class IAttribute;

template <typename AttrT, typename AttrF>
class GenericAttributeSupplier {
 public:
  GenericAttributeSupplier(const utils::U8String& name, int max_attrs,
                           const AttrF& attr_factory)
      : name_(name), max_attrs_(max_attrs), attr_factory_(attr_factory) {}
  virtual ~GenericAttributeSupplier() = default;

  utils::U8String name() const { return name_; }

  int attr_count() const { return attrs_.size(); }

  std::vector<IAttribute*> attributes() const {
    std::vector<IAttribute*> attrs;
    attrs.reserve(attrs_.size());

    for (auto& attr : attrs_) {
      attrs.push_back(attr.get());
    }
    return attrs;
  }

  int max_attrs() const { return max_attrs_; }

  IAttribute* addAttribute() {
    // TODO(lutts): when add in full state, should we throw expection
    // currently not, because the UI will ensure this will not happen
    if (attrs_.size() == static_cast<size_t>(max_attrs_)) return nullptr;

    auto attr = attr_factory_.createAttribute();
    if (attr) attrs_.push_back(std::unique_ptr<AttrT>(attr));

    return attr;
  }

  void removeAttribute(IAttribute* attr) {
    auto iter = std::find_if(begin(attrs_), end(attrs_),
                             [attr](const std::unique_ptr<AttrT>& item) {
                               return item.get() == attr;
                             });
    if (iter != end(attrs_)) {
      attrs_.erase(iter);
    }
  }

  void attributeChanged(IAttribute* attr) { AttributeChanged(attr); }

 public:
  SNAIL_SIGSLOT_PIMPL(IAttributeSupplier, AttributeChanged);

 private:
  SNAIL_DISABLE_COPY(GenericAttributeSupplier);

 private:
  utils::U8String name_;
  int max_attrs_;
  const AttrF& attr_factory_;
  std::vector<std::unique_ptr<AttrT> > attrs_;
};

}  // namespace snailcore

#endif  // SRC_CORE_GENERIC_ATTRIBUTE_SUPPLIER_H_
