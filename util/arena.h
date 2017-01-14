#ifndef UTIL_ARENA_H
#define UTIL_ARENA_H

#include <stddef.h>
#include <assert.h>
#include <vector>

namespace leveldb {

class Arena {
public:
  Arena();
  ~Arena();

  char *Allocate(size_t bytes);
private:
  char *AllocateFallback(size_t bytes);
  char *AllocateNewBlock(size_t block_bytes);

  char *alloc_ptr_;
  size_t alloc_bytes_remaining_;
  std::vector<char*> blocks_;
  size_t blocks_memory_;

  Arena(const Arena&);
  void operator=(const Arena&);
};

inline char *Arena::Allocate(size_t bytes) {
  assert(bytes > 0);
  if (bytes <= alloc_bytes_remaining_) {
    char *result = alloc_ptr_;
    alloc_ptr_ += bytes;
    alloc_bytes_remaining_ -= bytes;
    return result;
  }
  return AllocateFallback(bytes);
}
}

#endif // UTIL_ARENA_H

