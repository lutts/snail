# about signal proxy small buffer optimization #

Note: SBO is trade space for time, see also the Fast Pimpl idom

in header

```
class XXX {
...
  SignalBuffer signal_buffer_;
...
};
```

in cc file

```
class XXXSignalHelper {
 public:
  XXXSignalHelper(SignalBuffer* signal_buffer)
    : signal_buffer_(signal_buffer) {
    initializeSignalSSS();
  }
  ~XXXSignalHelper() {
    if (sss_) {
      sss_->~SSS_SignalType();
    }
  }

  void initializeSignalSSS() {
    void* storage = signal_buffer_->allocate(sizeof(SSS_SignalType));
    sss_ = new (storage) SSS_SignalType();
  }

  SSS_SignalType& getSSS() {
    return *sss_;
  }

  // the signal helper itself is also allocated from SignalBuffer
  operator new() {
    ....
  }

  operator delete() {
    ...
  }

 private:
  SignalBuffer* signal_buffer_;

  SSS_SignalType* sss_ { nullptr };
};

XXX::XXX() : signal_helper_(make_unique<XXXSignalHelper>()) {
}
```

so, SignalBuffer should have enough storage to store specified number
of signals and pointers (from signal helper), we may use a fixed stack size,
if no enough memory, allocate from heap
