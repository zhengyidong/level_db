#ifndef TABLE_FILTER_BLOCK_H
#define TABLE_FILTER_BLOCK_H

#include <stdint.h>

#include "leveldb/slice.h"

namespace leveldb {

class FilterPolicy;

class FilterBlockReader {
public:
  FilterBlockReader(const FilterPolicy *policy, const Slice &contents);
  bool KeyMayMatch(uint64_t block_offset, const Slice &key);
private:
  const FilterPolicy *policy_;
  const char *data_;
  const char *offset_;
  size_t num_;
  size_t base_lg_;
};
}
#endif // TABLE_FILTER_BLOCK_H

