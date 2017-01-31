#ifndef DB_DB_IMPL_H
#define DB_DB_IMPL_H

#include <deque>
#include "leveldb/db.h"
#include "port/port.h"

namespace leveldb {

class FileLock;
class VersionEdit;
class VersionSet;

class DBImpl : public DB {
public:
  DBImpl(const Options& options, const std::string& dbname);
  virtual ~DBImpl();
  virtual Status Put(const WriteOptions&, const Slice& key, const Slice& value);
  virtual Status Delete(const WriteOptions&, const Slice& key);
  virtual Status Write(const WriteOptions& options, WriteBatch* updates);
private:
  struct Writer;

  // Recover the descriptor from persistent storage.  May do a significant
  // amount of work to recover recently logged updates.  Any changes to
  // be made to the descriptor are added to *edit.
  Status Recover(VersionEdit *edit);

  Status MakeRoomForWrite(bool force);

  // Constant after construction
  Env *env_;
  const Options options_;
  const std::string dbname_;

  // Lock over the persistent DB state.  Non-NULL iff successfully acquired.
  FileLock *db_lock_;

  port::Mutex mutex_;
  std::deque<Writer*> writers_;

  VersionSet *versions_;

  Status bg_error_;
};
}

#endif // DB_DB_IMPL_H

