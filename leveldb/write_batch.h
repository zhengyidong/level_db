#ifndef LEVELDB_WRITE_BATCH_H
#define LEVELDB_WRITE_BATCH_H

#include <string>
#include "leveldb/status.h"

namespace leveldb {

class Slice;

class WriteBatch {
public:
  WriteBatch();
  ~WriteBatch();

  // Store the mapping "key->value" in the database.
  void Put(const Slice &key, const Slice &value);

  void Delete(const Slice &key);
  void Clear();

  class Handler {
  public:
    virtual ~Handler();
    virtual void Put(const Slice &key, const Slice &value) = 0;
    virtual void Delete(const Slice &key) = 0;
  };
  Status Iterate(Handler *handler) const;
private:
  friend class WriteBatchInternal;

  std::string rep_;  // See comment in write_batch.cc for the format of rep_
};
}

#endif // LEVELDB_WRITE_BATCH_H
