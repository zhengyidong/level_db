#ifndef LEVELDB_TABLE_H
#define LEVELDB_TABLE_H

#include <stdint.h>
#include "leveldb/iterator.h"

namespace leveldb {

struct Options;
class RandomAccessFile;

class Table {
public:
  static Status Open(const Options &options,
                     RandomAccessFile *file,
                     uint64_t file_size,
                     Table **table);
private:
  class Rep;
  Rep *rep_;

  explicit Table(Rep *rep) { rep_ = rep; }
};
}

#endif // LEVELDB_TABLE_H

