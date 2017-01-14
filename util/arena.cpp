#include "util/arena.h"

namespace leveldb {

static const int kBlockSize = 4096;

Arena::Arena()
{
  blocks_memory_ = 0;
  alloc_ptr_ = NULL;
  alloc_bytes_remaining_ = 0;
}

Arena::~Arena()
{
  for (size_t i=0; i < blocks_.size(); ++i) {
    delete[] blocks_[i];
  }
}

char *Arena::AllocateFallback(size_t bytes)
{
  if (bytes > kBlockSize / 4) {
    char *result = AllocateNewBlock(bytes);
    return result;
  }

  alloc_ptr_ = AllocateNewBlock(kBlockSize);
  alloc_bytes_remaining_ = kBlockSize;

  char *result = alloc_ptr_;
  alloc_ptr_ += bytes;
  alloc_bytes_remaining_ -= bytes;
  return result;
}

char *Arena::AllocateNewBlock(size_t block_bytes)
{
  char *result = new char[block_bytes];
  blocks_memory_ += block_bytes;
  blocks_.push_back(result);
  return result;
}

}
