// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <algorithm>
#include <vector>

#include "src/core/link_type.h"
#include "snail/i_attribute.h"
#include "utils/signal_slot_impl.h"
#include "utils/basic_utils.h"

namespace snailcore {

class LinkTypeSignalHelper {
 public:
  SNAIL_SIGSLOT_PIMPL(LinkType, LinkUpdated);

  friend class LinkType;
};

SNAIL_SIGSLOT_DELEGATE(LinkType, LinkUpdated, signal_helper_);

// region: constructor, destructor and assignments
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Weffc++"

LinkType::LinkType(const utils::U8String& name,
                   bool is_group_only)
    : signal_helper_{utils::make_unique<LinkTypeSignalHelper>()}
    , name_{name}
    , is_group_only_{is_group_only}
    , attr_suppliers_{ }
    , link_phrase_{ }
    , named_string_formatter_{ } { }

LinkType::LinkType(const LinkType& rhs)
    : signal_helper_{utils::make_unique<LinkTypeSignalHelper>()}
    , name_(rhs.name_)
    , is_group_only_(rhs.is_group_only_)
    , prototype_(rhs.getPrototype())
    , attr_suppliers_ { }
    , link_phrase_{rhs.link_phrase_}
    , named_string_formatter_{rhs.named_string_formatter_} {
  for (auto & supplier : rhs.attr_suppliers_) {
    std::unique_ptr<IAttributeSupplier> supplier_clone(supplier->clone());
    attr_suppliers_.push_back(std::move(supplier_clone));
  }
}

// signals are not move and copied
LinkType::LinkType(LinkType&& rhs)
    : signal_helper_{utils::make_unique<LinkTypeSignalHelper>()}
    , name_(std::move(rhs.name_))
    , is_group_only_(std::move(rhs.is_group_only_))
    , prototype_(rhs.getPrototype())  // copy
    , attr_suppliers_(std::move(rhs.attr_suppliers_))
    , link_phrase_{std::move(rhs.link_phrase_)}
    , named_string_formatter_{std::move(rhs.named_string_formatter_)} {
  rhs.is_group_only_ = false;
  rhs.prototype_ = nullptr;
}

LinkType& LinkType::operator=(LinkType rhs) {
  return swap(rhs);
}

// NOTE: signals not changed
LinkType& LinkType::swap(LinkType& rhs) noexcept {
  std::swap(name_, rhs.name_);
  std::swap(is_group_only_, rhs.is_group_only_);
  std::swap(prototype_, rhs.prototype_);
  std::swap(attr_suppliers_, rhs.attr_suppliers_);
  std::swap(link_phrase_, rhs.link_phrase_);
  std::swap(named_string_formatter_, rhs.named_string_formatter_);

  return *this;
}

LinkType::~LinkType() = default;

#pragma GCC diagnostic pop
// endregion: constructor, destructor and assignments

utils::U8String LinkType::name() const {
  return name_;
}

bool LinkType::isGroupOnly() const {
  return is_group_only_;
}

void LinkType::setAttributeSuppliers(
    std::vector<std::unique_ptr<IAttributeSupplier>>&& attr_suppliers) {
  attr_suppliers_ = std::move(attr_suppliers);
}

std::vector<IAttributeSupplier*> LinkType::attributeSuppliers() const {
  std::vector<IAttributeSupplier*> suppliers;

  for (auto & supplier : attr_suppliers_) {
    suppliers.push_back(supplier.get());
  }

  return suppliers;
}

const LinkType* LinkType::getPrototype() const {
  if (!prototype_)
    return this;
  else
    return prototype_;
}

const fto::LinkType* LinkType::prototype() const {
  return getPrototype();
}

utils::U8String LinkType::toString() const {
  return named_string_formatter_.format(link_phrase_, this);
}

void LinkType::clear() {
  if (!prototype_)
    return;

  *this = *prototype_;
}

std::vector<utils::U8String> LinkType::lookup(const utils::U8String& var_name) {
  std::vector<utils::U8String> result;

  for (auto & supplier : attr_suppliers_) {
    if (supplier->name() == var_name) {
      result.reserve(supplier->attr_count());
      auto attrs = supplier->attributes();
      for (auto & attr : attrs) {
        result.push_back(attr->valueText());
      }

      return result;
    }
  }

  return result;
}

void LinkType::setLinkPhrase(const utils::U8String& link_phrase) {
  link_phrase_ = link_phrase;
}

}  // namespace snailcore
