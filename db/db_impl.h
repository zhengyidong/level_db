#ifndef DB_DB_IMPL_H
#define DB_DB_IMPL_H

#include <deque>
#include "leveldb/db.h"
#include "port/port.h"

namespace leveldb {
class DBImpl : public DB {
public:
  DBImpl(const Options& options, const std::string& dbname);
  virtual ~DBImpl();
  virtual Status Put(const WriteOptions&, const Slice& key, const Slice& value);
  virtual Status Delete(const WriteOptions&, const Slice& key);
  virtual Status Write(const WriteOptions& options, WriteBatch* updates);
private:
  struct Writer;

  Status MakeRoomForWrite(bool force);

  port::Mutex mutex_;
  std::deque<Writer*> writers_;
  Status bg_error_;
};
}

#endif // DB_DB_IMPL_H

