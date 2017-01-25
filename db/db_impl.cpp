#include "db/db_impl.h"

#include "db/dbformat.h"
#include "db/write_batch_internal.h"

namespace leveldb {
Status DBImpl::Put(const WriteOptions &o, const Slice &key, const Slice &val) {
  return DB::Put(o, key, val);
}

Status DB::Put(const WriteOptions &opt, const Slice &key, const Slice &value) {
  WriteBatch batch;
  batch.Put(key, value);
  return Write(opt, &batch);
}

Status DB::Delete(const WriteOptions& opt, const Slice& key) {
  WriteBatch batch;
  batch.Delete(key);
  return Write(opt, &batch);
}

DB::~DB() { }

}
