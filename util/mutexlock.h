#ifndef UTIL_MUTEXLOCK_H
#define UTIL_MUTEXLOCK_H

#include "port/port.h"

namespace leveldb {
class MutexLock {
public:
  explicit MutexLock(port::Mutex *mu)
    : mu_(mu) {
    this->mu_->Lock();
  }
  ~MutexLock() { this->mu_->Unlock(); }
private:
  port::Mutex *const mu_;
  // No copying allowed
  MutexLock(const MutexLock&);
  void operator=(const MutexLock&);
};
}

#endif // UTIL_MUTEXLOCK_H

