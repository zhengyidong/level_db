#ifndef TABLE_BLOCK_H
#define TABLE_BLOCK_H

#include <stddef.h>
#include <stdint.h>
#include "leveldb/iterator.h"

namespace leveldb {

struct BlockContents;
class Comparator;

class Block {
public:
  explicit Block(const BlockContents &block);
  ~Block();

  size_t size() const { return size_; }
  Iterator *NewIterator(const Comparator *comparator);
private:
  uint32_t NumRestarts() const;

  const char *data_;
  size_t size_;
  uint32_t restart_offset_;     // Offset in data_ of restart array
  bool owned_;                  // Block owns data_[]

  // No copying allowed
  Block(const Block&);
  void operator=(const Block&);

  class Iter;
};
}

#endif // TABLE_BLOCK_H

