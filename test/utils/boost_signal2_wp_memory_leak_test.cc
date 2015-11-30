// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <boost/signals2.hpp>
#include <boost/version.hpp>

#include <iostream>    // std::cout
#include <memory>      // std::shared_ptr
#include <functional>  // std::function

class TrackObject {
 public:
  TrackObject() {
    std::cout << "construct TrackObject, data = " << data << std::endl;
  }
  ~TrackObject() { std::cout << "destruct TrackObject" << std::endl; }

  static void* operator new(std::size_t sz) {
    void* p = ::operator new(sz);
    std::cout << "custom new for size " << sz << ", got p " << p << std::endl;
    return p;
  }

  static void operator delete(void* p) {
    std::cout << "custom delete for p " << p << std::endl;
    return ::operator delete(p);
  }
  static void* operator new[](std::size_t sz) {
    std::cout << "custom new for size " << sz << std::endl;
    return ::operator new(sz);
  }

 private:
  char data[50];
};

class SignalEmitter {
 public:
  using SignalType = boost::signals2::signal<void()>;

  SignalEmitter() = default;
  virtual ~SignalEmitter() = default;

  boost::signals2::connection registerHandler(
      std::function<void()> func, std::shared_ptr<TrackObject> trackobj) {
    SignalType::slot_type handler(func);
    return signal_.connect(handler.track_foreign(trackobj));
  }

  void trigger() { signal_(); }

 private:
  SignalType signal_;
};

template <class T>
struct custom_allocator {
  typedef T value_type;
  custom_allocator() noexcept {}
  template <class U>
  custom_allocator(const custom_allocator<U>&) noexcept {}
  T* allocate(std::size_t n) {
    std::cout << "allocate " << n << " objects, object size is " << sizeof(T)
              << std::endl;
    return static_cast<T*>(::operator new(n * sizeof(T)));
  }
  void deallocate(T* p, std::size_t n) {
    std::cout << "deallocate " << n << " objects" << std::endl;
    ::operator delete(p);
  }
};

int main() {
  std::cout << "Boost " << BOOST_VERSION / 100000 << '.'
            << BOOST_VERSION / 100 % 1000 << '.' << BOOST_VERSION % 100
            << std::endl;

  std::cout << "---TestCase1: check wp will hold the memory---" << std::endl;

  custom_allocator<TrackObject> alloc;
  {
    std::weak_ptr<TrackObject> wp4;

    {
      auto p4 = std::allocate_shared<TrackObject>(alloc);
      wp4 = p4;
    }
    std::cout << "---TestCase1: expect to free the memory before this line, "
              << "but wp prevents it from being freed ---" << std::endl;
  }
  std::cout
      << "---TestCase1: the memory is only freed when weak_ptr is out of scope"
      << ", before this line ---" << std::endl;

  std::cout
      << "---TestCase2: check signal2 will prevent memory from being freed---"
      << std::endl;
  {
    SignalEmitter signal_emiiter;
    boost::signals2::connection conn;
    {
      auto trackobj = std::allocate_shared<TrackObject>(alloc);

      conn = signal_emiiter.registerHandler(
          []() { std::cout << "signal triggered" << std::endl; }, trackobj);
      std::cout << "---TestCase2: after registerHandler: conn.connected: "
                << conn.connected() << std::endl;
      signal_emiiter.trigger();
    }
    std::cout << "---TestCase2: should free trackobj here, but not---\n";
    std::cout << "---TestCase2: connected: " << conn.connected() << std::endl;
    std::cout
        << "---TestCase2: connected() shows that it's already disconnected"
        << std::endl;
    conn.disconnect();
    std::cout << "--TestCase2: so, conn.disconnect() does nothing, "
                 "the memory is still not freed"
              << std::endl;
    signal_emiiter.trigger();
    std::cout
        << "---TestCase2: only re-trigger will free memory before this line ---"
        << std::endl;
  }

  std::cout << "---TestCase3: check non-make_shared workaround---" << std::endl;
  {
    SignalEmitter signal_emiiter;
    {
      std::shared_ptr<TrackObject> trackobj(new TrackObject());

      signal_emiiter.registerHandler(
          []() { std::cout << "signal triggered" << std::endl; }, trackobj);
      signal_emiiter.trigger();
    }
    std::cout << "---TestCase3: without use make_shared, memory will be "
                 "freed before this line---"
              << std::endl;
    signal_emiiter.trigger();
    std::cout << "---TestCase3: re-trigger do nothing ---" << std::endl;
  }
}
