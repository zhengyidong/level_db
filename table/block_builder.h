#ifndef TABLE_BLOCK_BUILDER_H
#define TABLE_BLOCK_BUILDER_H

#include <vector>

#include <stdint.h>
#include "leveldb/slice.h"

namespace leveldb {

struct Options;

class BlockBuilder {
public:
  explicit BlockBuilder(const Options *options);

  void Reset();

  void Add(const Slice &key, const Slice &value);

  Slice Finish();

  size_t CurrentSizeEstimate() const;

  // Return true iff no entries have been added since the last Reset()
  bool empty() const {
    return buffer_.empty();
  }

private:
  const Options *options_;
  std::string           buffer_;      // Destination buffer
  std::vector<uint32_t> restarts_;    // Restart points
  int                   counter_;     // Number of entries emitted since restart
  bool                  finished_;    // Has Finish() been called?
  std::string last_key_;

  // No copying allowed
  BlockBuilder(const BlockBuilder&);
  void operator=(const BlockBuilder&);
};
}

#endif // TABLE_BLOCK_BUILDER_H

