#include "db/db_impl.h"

#include "db/filename.h"
#include "db/dbformat.h"
#include "db/version_set.h"
#include "db/write_batch_internal.h"
#include "leveldb/env.h"
#include "util/mutexlock.h"

namespace leveldb {

struct DBImpl::Writer {
  Status status;
  WriteBatch *batch;
  bool sync;
  bool done;
  port::CondVar cv;

  explicit Writer(port::Mutex *mu) : cv(mu) {}
};

Status DBImpl::Recover(VersionEdit *edit) {
  mutex_.AssertHeld();

  env_->CreateDir(dbname_);
  assert(db_lock_ == NULL);
  Status s = env_->LockFile(LockFileName(dbname_), &db_lock_);
  if (!s.ok()) {
    return s;
  }

  if (!env_->FileExists(CurrentFileName(dbname_))) {
    if (options_.create_if_missing) {
      // TODO
//      s = NewDB();
      if (!s.ok()) {
        return s;
      }
    } else {
      return Status::InvalidArgument(
            dbname_, "does not exist (create_if_missing is false)");
    }
  } else {
    if (options_.error_if_exists) {
      return Status::InvalidArgument(
            dbname_, "exists (error_if_exists is true)");
    }
  }
  // TODO
}

Status DBImpl::Put(const WriteOptions &o, const Slice &key, const Slice &val) {
  return DB::Put(o, key, val);
}

Status DBImpl::Delete(const WriteOptions& options, const Slice& key) {
  return DB::Delete(options, key);
}

Status DBImpl::Write(const WriteOptions &options, WriteBatch *updates) {
  Writer w(&mutex_);
  w.batch = updates;
  w.sync = options.sync;
  w.done = false;

  MutexLock l(&mutex_);
  writers_.push_back(&w);
  while (!w.done && &w != writers_.front()) {
    w.cv.Wait();
  }
  if (w.done) {
    return w.status;
  }


}

Status DBImpl::MakeRoomForWrite(bool force) {
  mutex_.AssertHeld();
  assert(!writers_.empty());
  bool allow_delay = !force;
  Status s;
  while (true) {
    if (!bg_error_.ok()) {
      s = bg_error_;
      break;
    } else if (
               allow_delay){}
  }
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

DB::~DB() {}

Status DB::Open(const Options &options, const std::string &name, DB **dbptr) {
  // TODO
  /*
  *dbptr = NULL;

  DBImpl *impl = new DBImpl(options, name);
  impl->mutex_.Lock();
  VersionEdit edit;
  */
}

}
