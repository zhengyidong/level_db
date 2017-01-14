#ifndef PORT_ATOMIC_POINTER_H
#define PORT_ATOMIC_POINTER_H

namespace leveldb {
namespace port {
inline void MemoryBarrier() {
  __asm__ __volatile__("" : : : "memory");
}
class AtomicPointer {
private:
  void* rep_;
public:
  AtomicPointer() { }
  explicit AtomicPointer(void* p) : rep_(p) {}
  inline void* NoBarrier_Load() const { return rep_; }
  inline void NoBarrier_Store(void* v) { rep_ = v; }
  inline void* Acquire_Load() const {
    void* result = rep_;
    MemoryBarrier();
    return result;
  }
  inline void Release_Store(void* v) {
    MemoryBarrier();
    rep_ = v;
  }
};
}
}


#endif // PORT_ATOMIC_POINTER_H

