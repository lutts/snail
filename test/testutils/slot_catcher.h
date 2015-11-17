// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_SLOT_CATCHER_H_
#define TEST_TESTUTILS_SLOT_CATCHER_H_

template <typename SlotType>
class SlotCatcher {
 public:
  SlotCatcher() {}
  virtual ~SlotCatcher() { cleanRawData(); }

  void operator=(const SlotType& slot) {
    cleanRawData();
    slot_ = new (raw_data) SlotType(slot);
  }

  template <typename... Args>
  typename SlotType::result_type operator()(Args... args) {
    if (!slot_) {
      throw std::runtime_error(
          "slot is not catched, see GMock error "
          "message for further information");
    }

    return slot_->operator()(args...);
  }

 private:
  SlotCatcher(const SlotCatcher& other) = delete;
  SlotCatcher& operator=(const SlotCatcher& other) = delete;

  void cleanRawData() {
    if (slot_) slot_->~SlotType();
  }

  unsigned char raw_data[sizeof(SlotType)];
  SlotType* slot_{nullptr};
};

#endif  // TEST_TESTUTILS_SLOT_CATCHER_H_
